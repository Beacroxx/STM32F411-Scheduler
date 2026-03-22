.syntax unified
.cpu cortex-m4
.thumb

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
    