/**
 * STM32F411CE BlackPill V2.0 With EEPROM
 *
 * EEPROM: W25Q64JV
 * /CS -> PA4
 * DO  -> PB4
 * /WP -> 3.3V
 * GND -> GND
 * DI  -> PA7
 * CLK -> PA5
 * HLD -> 3.3V
 * VCC -> 3.3V
 */

#include "filesystem.hpp"
#include "scheduler.hpp"
#include "spi.hpp"
#include "systick.hpp"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

extern "C" void sv_call_handler() {
  uint8_t svc_num;
  uint32_t *psp;
  __asm volatile("mrs %0, psp\n" : "=r"(psp));
  svc_num = *((uint8_t *)((*(psp + 6) & 0xfffffffeul) - 2));

  // do whatever idk
  (void)svc_num;
}

extern "C" void hard_fault_handler() {
  while (1) {
    gpio_toggle(GPIOC, GPIO13);
    for (volatile uint32_t i = 0; i < 4800000; i++) {
    } /* 50ms @ 96MHz AHB */
  }
}

int main() {
  // Setup system clock
  rcc_clock_setup_pll(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_96MHZ]);

  // Setup systick
  SysTick::init();

  // Enable GPIO clocks
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOC);

  // Set PC13 as output (push-pull)
  gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
  gpio_set_output_options(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO13);

  SPI::init();

  uint8_t id[3];
  SPI::cs_low();
  SPI::spi_write_buf((const uint8_t[]) {0x9F}, 1);
  SPI::spi_read_buf(id, 3);
  SPI::cs_high();

  // Check JEDEC ID for W25Q64JV (should be 0xEF 0x40 0x17)
  if (id[0] != 0xEF || id[1] != 0x40 || id[2] != 0x17) {
    hard_fault_handler();
  }

  Filesystem::init();

  while (1) {
    gpio_toggle(GPIOC, GPIO13); // Toggle LED
    // Wait 500ms using ms counter
    SysTick::delayMs(1000);
  }
  return 0;
}
