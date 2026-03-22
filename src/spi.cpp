#include "spi.hpp"

#include "scheduler.hpp"

#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>

void SPI::init() {
  rcc_periph_clock_enable(RCC_SPI1);

  // PA4 (NSS) Output
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO4);
  gpio_set(GPIOA, GPIO4);

  // PA5, PA7, PA6: SPI1
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO5 | GPIO6 | GPIO7);
  gpio_set_af(GPIOA, GPIO_AF5, GPIO5 | GPIO6 | GPIO7);

  spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_128, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE, SPI_CR1_CPHA_CLK_TRANSITION_1,
                  SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

  spi_enable_software_slave_management(SPI1);
  spi_enable_ss_output(SPI1);
  spi_enable(SPI1);
}

static void spi_transfer(const uint8_t *tx_buf, uint8_t *rx_buf, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    // Wait until TX buffer is empty
    while (!(SPI_SR(SPI1) & SPI_SR_TXE))
      ;
    spi_send(SPI1, tx_buf ? tx_buf[i] : 0xFF);
    // Wait until RX buffer is not empty
    while (!(SPI_SR(SPI1) & SPI_SR_RXNE))
      ;
    uint8_t data = spi_read(SPI1);
    if (rx_buf)
      rx_buf[i] = data;
  }
}

uint8_t SPI::spi_tx_rx(uint8_t tx) {
  uint8_t rx = 0;
  spi_transfer(&tx, &rx, 1);
  return rx;
}

void SPI::spi_write_buf(const uint8_t *buf, size_t len) {
  // Transmit buffer, discard RX
  spi_transfer(buf, nullptr, len);
}

void SPI::spi_read_buf(uint8_t *buf, size_t len) {
  // Receive buffer, send 0xFF as dummy
  spi_transfer(nullptr, buf, len);
}

void SPI::cs_low() { gpio_clear(GPIOA, GPIO4); }
void SPI::cs_high() { gpio_set(GPIOA, GPIO4); }