.syntax unified
.cpu cortex-m4
.thumb

/**
 * void SysTick::delayCyc(uint32_t)
 *
 * Does N*3 cycles of delay
 */
.global _ZN7SysTick8delayCycEm
.type _ZN7SysTick8delayCycEm, %function

_ZN7SysTick8delayCycEm:
  PUSH {r0}                 // Save r0

.L_delay:
  SUBS r0, r0, #1           // while (--r0)
  BNE .L_delay

  POP {r0}                  // Restore r0
  BX LR
