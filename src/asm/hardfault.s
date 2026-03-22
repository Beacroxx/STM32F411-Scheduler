.syntax unified
.cpu cortex-m4
.thumb

/**
 * void hard_fault_handler(void)
 *
 * HardFault handler.
 * Blinks PC13 in an infinite loop.
 */
.global hard_fault_handler
.type hard_fault_handler, %function

hard_fault_handler:
  LDR r0, =0x40020814  // GPIOC base + ODR offset
  LDR r1, [r0]

.L_loop:
  EOR r1, r1, #(1<<13) // Pin 13
  STR r1, [r0]

  LDR r2, =1600000     // ~50ms @ 96MHz AHB
.L_delay:
  SUBS r2, r2, #1
  BNE .L_delay

  B .L_loop
