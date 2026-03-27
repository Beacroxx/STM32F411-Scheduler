// STM32F411CE BlackPill V2.0 With EEPROM

#include "filesystem.hpp"
#include "malloc.hpp"
#include "scheduler.hpp"
#include "spi.hpp"
#include "systick.hpp"
#include "usb.hpp"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

void test() {
  gpio_toggle(GPIOC, GPIO13);
  SysTick::delayMs(1000);
  gpio_toggle(GPIOC, GPIO13);
  SysTick::delayMs(1000);
}

void test2() {
  while (1) {
    gpio_toggle(GPIOC, GPIO13);
    SysTick::delayMs(1500);
  }
}

int main() {
  // Setup system clock
  rcc_clock_setup_pll(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_96MHZ]);

  // Set SVC to prio 1 to allow long running syscalls
  nvic_set_priority(NVIC_SV_CALL_IRQ, 1);

  // Setup systick
  SysTick::init();

  // Enable GPIO clocks
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOC);

  // Set PC13 as output (push-pull)
  gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
  gpio_set_output_options(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO13);

  // Setup malloc
  Malloc::init();

  // Init USB
  USB::init();

  // Init SPI
  SPI::init();

  // Read JEDEC
  uint8_t id[3];
  SPI::cs_low();
  SPI::spi_write_buf((const uint8_t[]) {0x9F}, 1);
  SPI::spi_read_buf(id, 3);
  SPI::cs_high();

  // Check JEDEC ID for W25Q64JV (should be 0xEF 0x40 0x17)
  if (id[0] != 0xEF || id[1] != 0x40 || id[2] != 0x17) {
    printf("JEDEC ID did not match, expected 0xEF4017, got %02X%02X%02X\n", id[0], id[1], id[2]);
    hard_fault_handler();
  }

  // Init FS
  if (!Filesystem::init()) {
    printf("Failed to initizalize LittleFS");
    hard_fault_handler();
  }

  // Spawn Tasks
  Scheduler::createTask(test);
  Scheduler::createTask(test2);

  Scheduler::start();
}
