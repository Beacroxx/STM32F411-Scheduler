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

#include "scheduler.hpp"
#include "systick.hpp"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

extern "C" void HardFault_Handler() {
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

  // Enable GPIOC clock (for PC13)
  rcc_periph_clock_enable(RCC_GPIOC);

  // Set PC13 as output (push-pull)
  gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
  gpio_set_output_options(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO13);

  while (1) {
    gpio_toggle(GPIOC, GPIO13); // Toggle LED
    // Wait 500ms using ms counter
    SysTick::delayMs(1000);
  }
  return 0;
}
