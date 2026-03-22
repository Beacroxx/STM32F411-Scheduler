#include "sysio.hpp"

#include "scheduler.hpp"
#include "usb.hpp"

#include <cstring>
#include <libopencm3/usb/usbd.h>

extern "C" int _write(int file, char *ptr, int len) {
  if (file == 1 || file == 2) {
    int sent = 0;
    while (sent < len) {
      int chunk = (len - sent > 64) ? 64 : (len - sent);
      while (usbd_ep_write_packet(USB::usbd_dev, 0x82, ptr + sent, chunk) == 0)
        ;
      sent += chunk;
    }
    return len;
  }
  return -1;
}

extern "C" int _read(int file, char *ptr, int len) {
  if (file != 0)
    return -1;

  int read = 0;
  while (read < len) {
    while (USB::rx_head == USB::rx_tail)
      Scheduler::yield();

    uint16_t available = (USB::rx_head - USB::rx_tail) & (USB_RX_BUFFER_SIZE - 1);
    uint16_t to_read = (len - read > available) ? available : len - read;
    uint16_t first = (USB_RX_BUFFER_SIZE - USB::rx_tail > to_read) ? to_read : USB_RX_BUFFER_SIZE - USB::rx_tail;

    memcpy(ptr + read, (const void *)&USB::rx_buffer[USB::rx_tail], first);
    if (to_read > first)
      memcpy(ptr + read + first, (const void *)USB::rx_buffer, to_read - first);

    USB::rx_tail = (USB::rx_tail + to_read) & (USB_RX_BUFFER_SIZE - 1);
    read += to_read;
  }
  return read;
}

int readline(char *buf, int maxlen) {
  int i = 0;
  while (i < maxlen - 1) {
    char c;
    int n = _read(0, &c, 1);
    if (n <= 0)
      return -1;
    buf[i++] = c;
    if (c == '\n')
      break;
  }
  buf[i] = 0;
  return i;
}