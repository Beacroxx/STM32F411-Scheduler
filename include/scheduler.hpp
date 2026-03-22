#pragma once

#include <libopencm3/cm3/scb.h>

__attribute__((always_inline)) inline uint32_t __get_PSP(void) {
  uint32_t result;
  __asm__ volatile("MRS %0, psp" : "=r"(result));
  return (result);
}

__attribute__((always_inline)) inline void __set_PSP(uint32_t topOfProcStack) {
  __asm__ volatile("MSR psp, %0" : : "r"(topOfProcStack));
}

namespace Scheduler {
struct Task {
  uint32_t *sp;
  Task *next;
  Task *prev;
  uint32_t *sp_base;
};

extern Task *cur;

void yield();
void start();
void switchTasks();
void init();
void taskExit();
Task *createTask(void (*entry)(), uint32_t stackSize = 128);
} // namespace Scheduler