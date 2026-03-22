#include "scheduler.hpp"

#include <cstdlib>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>

Scheduler::Task *Scheduler::cur = nullptr;

void Scheduler::init() { nvic_set_priority(NVIC_PENDSV_IRQ, 255); }

void Scheduler::taskExit() {
  cur->prev->next = cur->next;
  cur->next->prev = cur->prev;

  free(cur->sp_base);
  cur->sp = nullptr;
  cur->sp_base = nullptr;

  // Pray to the UB gods that cur survives until task switches
  free(cur);

  while (1)
    Scheduler::yield();
}

Scheduler::Task *Scheduler::createTask(void (*entry)(), uint32_t stackSize) {
  Task *t = (Task *)malloc(sizeof(Task));

  t->sp_base = (uint32_t *)malloc(stackSize * sizeof(uint32_t));
  t->sp = t->sp_base + stackSize;

  // push task context
  *(--t->sp) = 0x01000000;         // xPSR
  *(--t->sp) = (uint32_t)entry;    // PC
  *(--t->sp) = (uint32_t)taskExit; // LR

  // Push registers (R12, R0-R3, R4–R11)
  for (int i = 0; i < 13; ++i)
    *(--t->sp) = 0;

  if (cur) {
    t->prev = cur;
    t->next = cur->next;
    cur->next->prev = t;
    cur->next = t;
  } else {
    t->next = t;
    t->prev = t;
    cur = t;
  }

  return t;
}