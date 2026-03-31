#pragma once

#include <cstddef>
#include <cstdint>

namespace UART {
#if COMM == 1
constexpr size_t BUFFER_SIZE = 512;

/**
 * @brief UART receive buffer for incoming data.
 */
extern volatile uint8_t rxBuffer[BUFFER_SIZE];
/**
 * @brief Head index for the UART receive buffer.
 */
extern volatile uint16_t rxHead;
/**
 * @brief Tail index for the UART receive buffer.
 */
extern volatile uint16_t rxTail;

/**
 * @brief UART transmit buffer for incoming data.
 */
extern volatile uint8_t txBuffer[BUFFER_SIZE];
/**
 * @brief Head index for the UART transmit buffer.
 */
extern volatile uint16_t txHead;
/**
 * @brief Tail index for the UART transmit buffer.
 */
extern volatile uint16_t txTail;
#endif

/**
 * @brief Initialize the UART peripheral and buffers.
 */
void init();

/**
 * @brief Receive data from the UART buffer.
 * @param dst Destination buffer to store received data.
 * @param len Maximum number of bytes to receive.
 * @return Number of bytes actually received.
 */
size_t recv(char *dst, size_t len);

/**
 * @brief Send data using the UART transmit buffer.
 * @param src Source buffer containing data to send.
 * @param len Number of bytes to send.
 * @return Number of bytes actually sent.
 */
size_t send(const char *src, size_t len);
} // namespace UART
