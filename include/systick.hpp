#pragma once

#include <stdint.h>

namespace SysTick {

/**
 * @brief Millisecond counter incremented by SysTick.
 */
extern volatile uint64_t ms;

/**
 * @brief Initialize the SysTick timer.
 */
void init();

/**
 * @brief Delay for a number of milliseconds.
 * @param delay Number of milliseconds.
 * @param start Start time in milliseconds.
 */
void delayMs(uint32_t delay, uint64_t start = ms);

/**
 * @brief Delay for a number of microseconds.
 * @param delay Number of microseconds.
 */
void delayUs(uint32_t delay);

/**
 * @brief Delay loop for a number of iterations, each taking 3-4 CPU cycles.
 * @param cycles Number of loop iterations.
 */
void delayCyc(uint32_t cycles);
}