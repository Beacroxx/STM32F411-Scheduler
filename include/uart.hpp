#pragma once

#include "types.hpp"

#include <cstddef>
#include <cstdint>

namespace UART {
#if COMM == 1
constexpr size_t BUFFER_SIZE = 512;

/**
 * @brief UART receive circular buffer for incoming data.
 */
extern Util::circBuffer<BUFFER_SIZE> rx;

/**
 * @brief UART transmit circular buffer for outgoing data.
 */
extern Util::circBuffer<BUFFER_SIZE> tx;
#endif

/**
 * @brief Flag to enable and disable echoing
 */
extern bool echo;

/**
 * @brief Initialize the UART peripheral.
 */
void init();

/**
 * @brief Receive data from the UART rx buffer.
 * @param dst Destination buffer to store received data.
 * @param len Maximum number of bytes to receive.
 * @return Number of bytes actually received.
 */
size_t recv(char *dst, size_t len);

/**
 * @brief Send data using the UART tx buffer.
 * @param src Source buffer containing data to send.
 * @param len Number of bytes to send.
 * @return Number of bytes actually sent.
 */
size_t send(const char *src, size_t len);
} // namespace UART
