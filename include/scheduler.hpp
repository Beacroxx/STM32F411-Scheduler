#pragma once

#include <libopencm3/cm3/scb.h>

namespace Scheduler {

/**
 * @brief Task Control Block for each registered task.
 */
struct TCB {
  uint32_t *sp; ///< Stack pointer for the task
  TCB *next;    ///< Next task in the list
  TCB *prev;    ///< Previous task in the list
};

/**
 * @brief Pointer to the currently running task.
 */
extern TCB *cur;

/**
 * @brief Yield control to the next task in the scheduler.
 */
void yield();

/**
 * @brief Start the scheduler and run tasks. Marked noreturn.
 */
void start() __attribute__((noreturn));

/**
 * @brief Create a new task and add it to the scheduler.
 * @param entry Entry point function for the task.
 * @param stackSize Stack size for the task (default 128).
 */
void createTask(void (*entry)(), uint32_t stackSize = 128);
} // namespace Scheduler