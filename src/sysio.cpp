#include "sysio.hpp"

#include "scheduler.hpp"
#include "uart.hpp"
#include "usb.hpp"

#include <cstring>
#include <libopencm3/usb/usbd.h>

extern "C" int _write(int file, char *ptr, int len) {
#if COMM == 2
  if (file == 1 || file == 2)
    return USB::send(ptr, len);
  return -1;
#elif COMM == 1
  if (file == 1 || file == 2)
    return UART::send(ptr, len);
  return -1;
#else
  return -1;
#endif
}

extern "C" int _read(int file, char *ptr, int len) {
  if (file != 0)
    return -1;
#if COMM == 2
  if (file == 1 || file == 2)
    return USB::recv(ptr, len);
  return -1;
#elif COMM == 1
  if (file == 1 || file == 2)
    return UART::recv(ptr, len);
  return -1;
#else
  return -1;
#endif
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