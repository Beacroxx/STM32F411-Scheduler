#pragma once

#include <libopencm3/usb/cdc.h>
#include <libopencm3/usb/usbd.h>

#define USB_RX_BUFFER_SIZE 512

namespace USB {
/**
 * @brief USB control transfer buffer.
 */
extern uint8_t control_buffer[128];
/**
 * @brief USB receive buffer for incoming data.
 */
extern volatile uint8_t rx_buffer[USB_RX_BUFFER_SIZE];
/**
 * @brief Head index for the USB receive buffer.
 */
extern volatile uint16_t rx_head;
/**
 * @brief Tail index for the USB receive buffer.
 */
extern volatile uint16_t rx_tail;
/**
 * @brief USB string descriptors array.
 */
extern const char *const usb_strings[];
/**
 * @brief Pointer to the USB device structure.
 */
extern usbd_device *usbd_dev;

/**
 * @brief Initialize the USB device and CDC interface.
 */
void init();
} // namespace USB