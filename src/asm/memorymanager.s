	.syntax unified
	.cpu cortex-m4
	.thumb

/**
 * void *MM::memcpy(void *dst, const void *src, size_t n)
 *
 * Fast quad-word-copy memcpy for STM32F411 (Cortex-M4)
 */
.global _ZN2MM6memcpyEPvPKvj
.type _ZN2MM6memcpyEPvPKvj, %function

_ZN2MM6memcpyEPvPKvj:
	PUSH    {r0, r4, r5, r6}
	CMP     r2, #0
	BEQ     .L_cpy_end

// Align dst to 4 bytes
.L_cpy_align:
	TST     r0, #3
	BEQ     .L_cpy_word
	CMP     r2, #0
	BEQ     .L_cpy_end
	LDRB    r4, [r1], #1
	STRB    r4, [r0], #1
	SUBS    r2, r2, #1
	B       .L_cpy_align

// Copy words
.L_cpy_word:
	MOVS    r4, r2
	LSRS    r4, r4, #4
	BEQ     .L_cpy_bytes
.L_cpy_qword_copy:
  LDMIA   r1!, {r3-r6}
  STMIA   r0!, {r3-r6}
	SUBS    r4, r4, #1
	BNE     .L_cpy_qword_copy
	ANDS    r2, r2, #15

// Copy remaining bytes
.L_cpy_bytes:
	CMP     r2, #0
	BEQ     .L_cpy_end
.L_cpy_byte_copy:
	LDRB    r4, [r1], #1
	STRB    r4, [r0], #1
	SUBS    r2, r2, #1
	BNE     .L_cpy_byte_copy

.L_cpy_end:
	POP     {r0, r4, r5, r6}
  BX LR

/**
 * void *MM::memset(void *ptr, uint8_t value, size_t n)
 *
 * Fast quad-word-fill memset for STM32F411 (Cortex-M4)
 */
.global _ZN2MM6memsetEPvhj
.type _ZN2MM6memsetEPvhj, %function

_ZN2MM6memsetEPvhj:
	PUSH    {r0, r4, r5}
	CMP     r2, #0
	BEQ     .L_set_end

	// Prepare word value in r4
	ORR     r4, r1, r1, LSL #8
	ORR     r4, r4, r4, LSL #16

// Align ptr to 4 bytes
.L_set_align:
	TST     r0, #3
	BEQ     .L_set_word
	CMP     r2, #0
	BEQ     .L_set_end
	STRB    r1, [r0], #1
	SUBS    r2, r2, #1
	B       .L_set_align

// Write words
.L_set_word:
	MOVS    r5, r2
	LSRS    r5, r5, #4
	BEQ     .L_set_bytes
.L_set_qword_fill:
	STR     r4, [r0], #4
	STR     r4, [r0], #4
	STR     r4, [r0], #4
	STR     r4, [r0], #4
	SUBS    r5, r5, #1
	BNE     .L_set_qword_fill
	ANDS    r2, r2, #15

// Write remaining bytes
.L_set_bytes:
	CMP     r2, #0
	BEQ     .L_set_end
.L_set_byte_fill:
	STRB    r1, [r0], #1
	SUBS    r2, r2, #1
	BNE     .L_set_byte_fill

.L_set_end:
	POP     {r0, r4, r5}
  BX LR
