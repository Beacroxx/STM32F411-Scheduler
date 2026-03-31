#pragma once

#include <cstddef>
#include <libopencm3/usb/cdc.h>
#include <libopencm3/usb/usbd.h>

namespace USB {
#if COMM == 2
constexpr size_t RX_BUFFER_SIZE = 512;

/**
 * @brief USB control transfer buffer.
 */
extern uint8_t control_buffer[128];
/**
 * @brief USB receive buffer for incoming data.
 */
extern volatile uint8_t rxBuffer[RX_BUFFER_SIZE];
/**
 * @brief Head index for the USB receive buffer.
 */
extern volatile uint16_t rxHead;
/**
 * @brief Tail index for the USB receive buffer.
 */
extern volatile uint16_t rxTail;
/**
 * @brief USB string descriptors array.
 */
extern const char *const usb_strings[];
/**
 * @brief Pointer to the USB device structure.
 */
extern usbd_device *usbd_dev;
#endif

/**
 * @brief Initialize the USB device and CDC interface.
 */
void init();

/**
 * @brief Receive data from the USB buffer.
 * @param dst Destination buffer to store received data.
 * @param len Maximum number of bytes to receive.
 * @return Number of bytes actually received.
 */
size_t recv(char *dst, size_t len);

/**
 * @brief Send data over USB.
 * @param src Source buffer containing data to send.
 * @param len Number of bytes to send.
 * @return Number of bytes actually sent.
 */
size_t send(const char *src, size_t len);
} // namespace USB