#include "scheduler.hpp"

#include "memorymanager.hpp"

#include <cstdlib>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>

Scheduler::TCB *Scheduler::cur = nullptr;

static void taskExit() {
  using namespace Scheduler;

  cur->prev->next = cur->next;
  cur->next->prev = cur->prev;
  cur->sp = nullptr;

  MM::free(cur);
  while (1)
    yield();
}

void Scheduler::createTask(void (*entry)(), uint32_t stackSize) {
  TCB *tcb = static_cast<TCB *>(MM::malloc(sizeof(TCB) + stackSize * sizeof(uint32_t)));

  // Push task context
  tcb->sp = reinterpret_cast<uint32_t *>(reinterpret_cast<uint32_t>(tcb) + sizeof(TCB)) + stackSize;
  *(--tcb->sp) = 0x01000000;                           // xPSR
  *(--tcb->sp) = reinterpret_cast<uint32_t>(entry);    // PC
  *(--tcb->sp) = reinterpret_cast<uint32_t>(taskExit); // LR

  // Push registers (R12, R0-R3, R4–R11)
  for (int i = 0; i < 13; ++i)
    *(--tcb->sp) = 0;

  if (!cur) {
    tcb->next = tcb;
    tcb->prev = tcb;
    cur = tcb;
    return;
  }

  tcb->prev = cur;
  tcb->next = cur->next;
  cur->next->prev = tcb;
  cur->next = tcb;
}