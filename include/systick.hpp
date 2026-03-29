#pragma once

#include <stdint.h>

namespace SysTick {
constexpr uint32_t TICKS_PER_US = 96;

extern volatile uint64_t ms;

void init();
void delayMs(uint32_t delay, uint64_t start = ms);
void delayUs(uint32_t delay);
void delayCyc(uint32_t cycles);
}