.syntax unified
.cpu cortex-m4
.thumb


/**
 * void Scheduler::yield()
 *
 * Requests a context switch by triggering the PendSV exception.
 */
.global _ZN9Scheduler5yieldEv
.type _ZN9Scheduler5yieldEv, %function

_ZN9Scheduler5yieldEv:
  LDR r0, =0xE000ED04          // ICSR
  LDR r1, =0x10000000          // PENDSVSET bit
  STR r1, [r0]
  BX LR

/**
 * void Scheduler::start()
 *
 * Initializes the first task’s stack and enters thread mode with PSP.
 */
.global _ZN9Scheduler5startEv
.type _ZN9Scheduler5startEv, %function

_ZN9Scheduler5startEv:
  LDR r0, =_ZN9Scheduler3curE
  LDR r0, [r0]
  LDR r0, [r0]
  MSR psp, r0

  MOV r0, #2
  MSR CONTROL, r0

  ISB

  POP {r4-r11}
  POP {r0-r3}
  POP {r12}
  POP {LR}
  POP {PC}


/**
 * void pend_sv_handler(void)
 *
 * PendSV interrupt handler. Saves current task state and restores the next one.
 */
.global pend_sv_handler
.type pend_sv_handler, %function

pend_sv_handler:
  CPSID I

  LDR r1, =_ZN9Scheduler3curE
  LDR r2, [r1]
  CMP r2, #0
  BEQ exit

  LDR r3, [r2, #4]
  LDR r0, [r2]
  CMP r0, #0
  BEQ skip_save

  MRS r0, psp
  STMDB r0!, {r4-r11}
  STR r0, [r2]

skip_save:
  STR r3, [r1]

  LDR r0, [r1]
  LDR r0, [r0]
  LDMIA r0!, {r4-r11}
  MSR psp, r0

exit:
  CPSIE I
  BX LR
