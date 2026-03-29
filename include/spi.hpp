#pragma once
#include <cstddef>
#include <cstdint>

namespace SPI {

/**
 * @brief Initialize the SPI peripheral.
 */
void init();

/**
 * @brief Transmit and receive a byte over SPI.
 * @param tx Byte to transmit.
 * @return Received byte.
 */
uint8_t spi_tx_rx(uint8_t tx);

/**
 * @brief Write a buffer over SPI.
 * @param buf Pointer to buffer to write.
 * @param len Number of bytes to write.
 */
void spi_write_buf(const uint8_t *buf, size_t len);

/**
 * @brief Read a buffer over SPI.
 * @param buf Pointer to buffer to fill.
 * @param len Number of bytes to read.
 */
void spi_read_buf(uint8_t *buf, size_t len);

/**
 * @brief Set chip select (CS) low.
 */
void cs_low();

/**
 * @brief Set chip select (CS) high.
 */
void cs_high();
} // namespace SPI