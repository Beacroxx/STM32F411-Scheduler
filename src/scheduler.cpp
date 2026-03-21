#include "scheduler.hpp"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>

void Scheduler::init() { nvic_set_priority(NVIC_PENDSV_IRQ, 255); }

Scheduler::Task *Scheduler::createTask(void (*entry)()) {
  //
}

extern "C" void pend_sv_handler() {}