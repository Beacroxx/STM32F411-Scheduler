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

/**
 * void sv_call_handler(void)
 * 
 * SVC (Supervisor Call) exception handler.
 * Extracts the SVC number and up to four arguments from the stack frame,
 * then calls the C-level syscall dispatcher.
 */
.global sv_call_handler
.type sv_call_handler, %function

sv_call_handler:
    TST LR, #4
    ITE EQ
    MRSEQ r4, msp
    MRSNE r4, psp

    LDR r1, [r4, #24]
    LDRB r0, [r1, #-2]
    LDR r1, [r4, #0]
    LDR r2, [r4, #4]
    LDR r3, [r4, #8]
    LDR r4, [r4, #12]

    PUSH {r4, LR}
    BL syscall
    POP {r4, LR}
    BX LR
    