#pragma once

#include <libopencm3/usb/cdc.h>
#include <libopencm3/usb/usbd.h>

#define USB_RX_BUFFER_SIZE 512

namespace USB {
extern uint8_t control_buffer[128];
extern volatile uint8_t rx_buffer[USB_RX_BUFFER_SIZE];
extern volatile uint16_t rx_head;
extern volatile uint16_t rx_tail;
extern const char *const usb_strings[];
extern usbd_device *usbd_dev;
void init();
} // namespace USB