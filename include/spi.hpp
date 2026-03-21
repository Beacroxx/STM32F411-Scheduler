#pragma once
#include <cstddef>
#include <cstdint>

namespace SPI {
void init();
uint8_t spi_tx_rx(uint8_t tx);
void spi_write_buf(const uint8_t *buf, size_t len);
void spi_read_buf(uint8_t *buf, size_t len);

void cs_low();
void cs_high();
} // namespace SPI