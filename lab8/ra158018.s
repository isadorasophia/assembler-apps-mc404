@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @
@                                                                             @
@                                    Lab. 8                                   @
@   Author: Isadora Sophia Garcia Rodopoulos                                  @
@   RA: 158018                                                                @
@                                                                             @
@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @

.globl _start               @ initial set up

@@@@@@@@@
@@@@@@@@@ interrupt vector section
@@@@@@@@@
.org 0x0
.section .iv, "a"

_start:
interrupt_vector:
    b RESET_HANDLER

.org 0x18
    b IRQ_HANDLER


@@@@@@@@@
@@@@@@@@@ code section
@@@@@@@@@
.org 0x100
.text

    @ set counter to 0
    ldr r2, =CONTADOR       @ stores counter address
    mov r0, #0
    str r0, [r2]

RESET_HANDLER:
    @ set interrupt table base address on coprocessor 15
    ldr r0, =interrupt_vector
    mcr p15, 0, r0, c12, c0, 0

SET_GPT:
    @ constants
    .set CLOCK_SRC,             0x41
    .set GPT_BASE,              0x53FA0000
    .set GPT_CR,                0x0
    .set GPT_PR,                0x4
    .set GPT_SR,                0x8
    .set GPT_OCR1,              0x10
    .set GPT_IR,                0xC

    ldr r1, =GPT_BASE

    ldr  r0, =CLOCK_SRC
    str r0, [r1, #GPT_CR]       @ set clock

    mov r0, #0
    str r0, [r1, #GPT_PR]       @ prescaler

    mov r0, #0x64
    str r0, [r1, #GPT_OCR1]     @ GPT_OCR1 = counting_max

    mov r0, #1
    str r0, [r1, #GPT_IR]       @ enable output compare channel 1

SET_TZIC:
    @ constants
    .set TZIC_BASE,             0x0FFFC000
    .set TZIC_INTCTRL,          0x0
    .set TZIC_INTSEC1,          0x84 
    .set TZIC_ENSET1,           0x104
    .set TZIC_PRIOMASK,         0xC
    .set TZIC_PRIORITY9,        0x424

    @ liga o controlador de interrupcoes
    @ R1 <= TZIC_BASE

    ldr r1, =TZIC_BASE

    @ configura interrupcao 39 do GPT como nao segura
    mov r0, #(1 << 7)
    str r0, [r1, #TZIC_INTSEC1]

    @ habilita interrupcao 39 (GPT)
    @ reg1 bit 7 (gpt) 

    mov r0, #(1 << 7)
    str r0, [r1, #TZIC_ENSET1]

    @ configure interrupt39 priority as 1
    @ reg9, byte 3

    ldr r0, [r1, #TZIC_PRIORITY9]
    bic r0, r0, #0xFF000000
    mov r2, #1
    orr r0, r0, r2, lsl #24
    str r0, [r1, #TZIC_PRIORITY9]

    @ configure PRIOMASK as 0
    eor r0, r0, r0
    str r0, [r1, #TZIC_PRIOMASK]

    @ habilita o controlador de interrupcoes
    mov r0, #1
    str r0, [r1, #TZIC_INTCTRL]

    @ instrucao msr - habilita interrupcoes
    msr CPSR_c, #0x13       @ SUPERVISOR mode, IRQ/FIQ enabled

loop:
    b loop                  @ infinite loop

IRQ_HANDLER:
    @ set GPT flag to 1
    mov r0, #1
    ldr r1, =GPT_BASE
    str r0, [r1, #GPT_SR]

    @ adds counter by 1
    ldr r0, =CONTADOR
    ldr r1, [r0]
    add r1, #1
    str r1, [r0]

    @ return
    movs pc, lr

@@@@@@@@@
@@@@@@@@@ data section
@@@@@@@@@
.org 0x400
.data

CONTADOR:   .word 0
