#include "systick.hpp"

#include "scheduler.hpp"

#include <libopencm3/cm3/dwt.h>
#include <libopencm3/cm3/systick.h>

volatile uint64_t SysTick::ms = 0;

void SysTick::init() {
  // 1ms Systick
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
  systick_set_reload(96000 - 1);
  systick_clear();
  systick_counter_enable();
  systick_interrupt_enable();

  // DWT
  dwt_enable_cycle_counter();
}

void SysTick::delayMs(uint32_t delay) {
  uint64_t start = ms;
  while ((ms - start) < delay)
    Scheduler::yield();
}

void SysTick::delayUs(uint32_t delay) {
  if (delay >= 1000) {
    SysTick::delayMs(delay / 1000);
    delay = delay % 1000;
  }
  if (delay == 0)
    return;

  uint32_t start = dwt_read_cycle_counter();
  uint32_t ticks = start + delay * TICKS_PER_US;
  while (dwt_read_cycle_counter() - start < ticks)
    ;
}

extern "C" void sys_tick_handler() {
  Scheduler::yield();
  SysTick::ms++;
}
