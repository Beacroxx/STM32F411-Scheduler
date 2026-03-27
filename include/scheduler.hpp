#pragma once

#include <libopencm3/cm3/scb.h>

namespace Scheduler {
struct TCB {
  uint32_t *sp;
  TCB *next;
  TCB *prev;
};

extern TCB *cur;

void yield() __attribute__((naked));
void start() __attribute__((noreturn, naked));
void switchTasks() __attribute__((naked));
void taskExit() __attribute__((noreturn));
void createTask(void (*entry)(), uint32_t stackSize = 128);
} // namespace Scheduler