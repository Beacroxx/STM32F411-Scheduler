#pragma once

#include <libopencm3/cm3/scb.h>

namespace Scheduler {
struct Task {
  void (*entry)();
  uint32_t *sp;
  uint32_t *sp_base;
  Task *next;
  Task *prev;
};

inline void yield() { SCB_ICSR |= SCB_ICSR_PENDSVSET; };
void init();
Task *createTask(void (*entry)());
} // namespace Scheduler