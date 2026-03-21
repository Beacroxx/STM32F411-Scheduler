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

  // PA5, PA7, PB4: SPI1
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO5 | GPIO6 | GPIO7);
  gpio_set_af(GPIOA, GPIO_AF5, GPIO5 | GPIO6 | GPIO7);

  spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_256, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE, SPI_CR1_CPHA_CLK_TRANSITION_1,
                  SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

  spi_enable_software_slave_management(SPI1);
  spi_enable_ss_output(SPI1);
  spi_enable(SPI1);
}

// Helper: DMA transfer for full-duplex SPI (blocking, yields to scheduler)
static void spi_dma_transfer(const uint8_t *tx_buf, uint8_t *rx_buf, size_t len) {
  if (len == 0)
    return;

  // Enable DMA2 clock
  rcc_periph_clock_enable(RCC_DMA2);

  // Disable streams before config
  dma_stream_reset(DMA2, DMA_STREAM3); // SPI1_TX
  dma_stream_reset(DMA2, DMA_STREAM0); // SPI1_RX

  // Configure RX (DMA2 Stream 0, Channel 3)
  dma_set_peripheral_address(DMA2, DMA_STREAM0, (uint32_t)&SPI_DR(SPI1));
  dma_set_memory_address(DMA2, DMA_STREAM0, (uint32_t)rx_buf);
  dma_set_number_of_data(DMA2, DMA_STREAM0, len);
  dma_channel_select(DMA2, DMA_STREAM0, DMA_SxCR_CHSEL_3);
  dma_set_priority(DMA2, DMA_STREAM0, DMA_SxCR_PL_HIGH);
  dma_set_memory_size(DMA2, DMA_STREAM0, DMA_SxCR_MSIZE_8BIT);
  dma_set_peripheral_size(DMA2, DMA_STREAM0, DMA_SxCR_PSIZE_8BIT);
  dma_enable_memory_increment_mode(DMA2, DMA_STREAM0);
  dma_set_transfer_mode(DMA2, DMA_STREAM0, DMA_SxCR_DIR_PERIPHERAL_TO_MEM);
  dma_enable_circular_mode(DMA2, DMA_STREAM0);
  dma_enable_direct_mode(DMA2, DMA_STREAM0);
  dma_enable_transfer_complete_interrupt(DMA2, DMA_STREAM0);

  // Configure TX (DMA2 Stream 3, Channel 3)
  dma_set_peripheral_address(DMA2, DMA_STREAM3, (uint32_t)&SPI_DR(SPI1));
  dma_set_memory_address(DMA2, DMA_STREAM3, (uint32_t)tx_buf);
  dma_set_number_of_data(DMA2, DMA_STREAM3, len);
  dma_channel_select(DMA2, DMA_STREAM3, DMA_SxCR_CHSEL_3);
  dma_set_priority(DMA2, DMA_STREAM3, DMA_SxCR_PL_HIGH);
  dma_set_memory_size(DMA2, DMA_STREAM3, DMA_SxCR_MSIZE_8BIT);
  dma_set_peripheral_size(DMA2, DMA_STREAM3, DMA_SxCR_PSIZE_8BIT);
  dma_enable_memory_increment_mode(DMA2, DMA_STREAM3);
  dma_set_transfer_mode(DMA2, DMA_STREAM3, DMA_SxCR_DIR_MEM_TO_PERIPHERAL);
  dma_enable_circular_mode(DMA2, DMA_STREAM3);
  dma_enable_direct_mode(DMA2, DMA_STREAM3);
  dma_enable_transfer_complete_interrupt(DMA2, DMA_STREAM3);

  // Enable SPI DMA requests
  spi_enable_rx_dma(SPI1);
  spi_enable_tx_dma(SPI1);

  // Enable DMA streams
  dma_enable_stream(DMA2, DMA_STREAM0);
  dma_enable_stream(DMA2, DMA_STREAM3);

  // Wait for both streams to finish, yielding to scheduler
  while (!(dma_get_interrupt_flag(DMA2, DMA_STREAM3, DMA_TCIF))) {
    Scheduler::yield();
  }
  while (!(dma_get_interrupt_flag(DMA2, DMA_STREAM0, DMA_TCIF))) {
    Scheduler::yield();
  }

  // Disable DMA streams
  dma_disable_stream(DMA2, DMA_STREAM0);
  dma_disable_stream(DMA2, DMA_STREAM3);

  // Disable SPI DMA requests
  spi_disable_rx_dma(SPI1);
  spi_disable_tx_dma(SPI1);

  // Clear transfer complete flags
  dma_clear_interrupt_flags(DMA2, DMA_STREAM0, DMA_TCIF);
  dma_clear_interrupt_flags(DMA2, DMA_STREAM3, DMA_TCIF);
}

uint8_t SPI::spi_tx_rx(uint8_t tx) {
  uint8_t rx = 0;
  spi_dma_transfer(&tx, &rx, 1);
  return rx;
}

void SPI::spi_write_buf(const uint8_t *buf, size_t len) {
  // Use DMA for TX, discard RX
  uint8_t dummy_rx[len];
  spi_dma_transfer(buf, dummy_rx, len);
}

void SPI::spi_read_buf(uint8_t *buf, size_t len) {
  // Use DMA for RX, send 0xFF as dummy
  uint8_t dummy_tx[len];
  for (size_t i = 0; i < len; ++i)
    dummy_tx[i] = 0xFF;
  spi_dma_transfer(dummy_tx, buf, len);
}

void SPI::cs_low() { gpio_clear(GPIOA, GPIO4); }
void SPI::cs_high() { gpio_set(GPIOA, GPIO4); }