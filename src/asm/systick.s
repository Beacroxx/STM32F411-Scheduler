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
.L_delay:
  SUBS r0, r0, #1
  BNE .L_delay

  BX LR

/**
 * void sys_tick_handler(void)
 *
 * Runs every millisecond
 */
.global sys_tick_handler
.type sys_tick_handler, %function

sys_tick_handler:
    PUSH {LR}
    BL _ZN9Scheduler5yieldEv

    LDR r0, =_ZN7SysTick2msE
    LDR r1, [r0]
    LDR r2, [r0, #4]
    ADDS r1, r1, #1
    ADC r2, r2, #0
    STR r1, [r0]
    STR r2, [r0, #4]

    POP {PC}
