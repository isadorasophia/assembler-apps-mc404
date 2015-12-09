@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @
@                                                                             @
@                        SOUL (Sistema Operacional UoLi)                      @
@   Authors: Isadora Sophia Garcia Rodopoulos                                 @
@            Matheus Diamantino                                               @
@   RA: 158018                                                                @
@       156740                                                                @
@                                                                             @
@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @

.org 0x0
.section .iv, "a"

_start:     
interrupt_vector:
    b RESET_HANDLER

.org 0x08
    b SYSCALL_HANDLER

.org 0x18
    b IRQ_HANDLER

@@@@@@@@@
@@@@@@@@@ constants
@@@@@@@@@
.text
@ address of GPT registers
    .set GPT_BASE,              0x53FA0000
    .set GPT_CR,                0x0
    .set GPT_PR,                0x4
    .set GPT_SR,                0x8
    .set GPT_OCR1,              0x10
    .set GPT_IR,                0xC
    .set GPT_CNT,               0x24

    .set CLOCK_SRC,             0x41            @ enable clock
    .set TIME_SZ,               100             @ clock cycling (1ms)
    .set CLOCK_WAIT,            200
    
@ addresses of TZIC registers
    .set TZIC_BASE,             0x0FFFC000
    .set TZIC_INTCTRL,          0x0
    .set TZIC_INTSEC1,          0x84 
    .set TZIC_ENSET1,           0x104
    .set TZIC_PRIOMASK,         0xC
    .set TZIC_PRIORITY9,        0x424

@ address of GPIO registers
    .set GPIO_BASE,             0x53F84000
    .set GPIO_DR,               0x0
    .set GPIO_GDIR,             0x4
    .set GPIO_PSR,              0x8

@ mask values according to the config.
    .set GDIR_MASK,             0b11111111111111000000000000111110

    .set MOTOR_0_MASK,          0b11111110000000111111111111111111
    .set MOTOR_1_MASK,          0b00000001111111111111111111111111
  
    .set SPEED_VALUE_MASK,      0b00000000000000000000000000111111

    .set SONAR_ID_MASK,         0b00000000000000000000000000001111

    .set SONAR_DATA_MASK,       0b00000000000000000000111111111111

    .set IRQ_DISABLE,           0b10011111

@ default value for user .text
    .set USER_TEXT,             0x77802a00

@ max. number of callbacks
    .set MAX_CALLBACKS,         8

@ max. number of alarms
    .set MAX_ALARMS,            8

@ max. pointers for execution
    .set MAX_EXECUTION,         16

@ stack pointer size
    .set STACK_SIZE,            1024

@ no. of sensors
    .set MAX_SENSOR_ID,         15

@ max speed
    .set MAX_SPEED,             63

@ amount to shift 32 bits
    .set BYTE,                  2

@@@@@@@@@
@@@@@@@@@ initialize system
@@@@@@@@@
RESET_HANDLER:
    @ set counter to 0
    ldr r2, =COUNTER
    mov r0, #0
    str r0, [r2]

    ldr r2, =FUN_ENABLED
    mov r0, #1
    str r0, [r2]

    ldr r0, =interrupt_vector
    mcr p15, 0, r0, c12, c0, 0      @ set interrupt table base address 
                                    @ on coprocessor 15

SET_GPT:
    ldr r1, =GPT_BASE               @ r1 <= GPT_BASE

    ldr r0, =CLOCK_SRC
    str r0, [r1, #GPT_CR]           @ set clock_scr to default

    mov r0, #0
    str r0, [r1, #GPT_PR]           @ set prescaler to zero

    ldr r0, =TIME_SZ
    str r0, [r1, #GPT_OCR1]         @ set GPT_OCR1 to counting value

    mov r0, #1
    str r0, [r1, #GPT_IR]           @ enable output compare channel 1

SET_TZIC:
    ldr r1, =TZIC_BASE              @ r1 <= TZIC_BASE

    mov r0, #(1 << 7)
    str r0, [r1, #TZIC_INTSEC1]     @ set interrupt39 from GPT as non-safe

    mov r0, #(1 << 7)               @ reg1, bit 7 (gpt)
    str r0, [r1, #TZIC_ENSET1]      @ enables interrupt39

    ldr r0, [r1, #TZIC_PRIORITY9]
    bic r0, r0, #0xFF000000
    mov r2, #1                      @ set interrupt39 priority as 1
    orr r0, r0, r2, lsl #24         @ reg9, byte 3
    str r0, [r1, #TZIC_PRIORITY9]

    eor r0, r0, r0
    str r0, [r1, #TZIC_PRIOMASK]    @ set PRIOMASK to 0

    mov r0, #1
    str r0, [r1, #TZIC_INTCTRL]     @ enables the interruption controller

    msr CPSR_c, #0x13               @ SUPERVISOR mode, IRQ/FIQ enabled
                                    @ enables interruptions during program

SET_GPIO:
    ldr r1, =GPIO_BASE              @ r1 <= GPIO_BASE

    ldr r0, =GDIR_MASK
    str r0, [r1, #GPIO_GDIR]        @ set GDIR according to the pins of 
                                    @ the hardware

SET_STACK_POINTERS:
    ldr r1, =STACK_P                @ r1 <= STACK
    
    add r1, r1, #1024

    msr CPSR_c, #0x12               @ IRQ mode
    mov sp, r1

    add r1, r1, #1024

    msr CPSR_c, #0x1F               @ SYSTEM mode
    mov sp, r1

    add r1, r1, #1024

    msr CPSR_c, #0x13               @ Supervisor mode
    mov sp, r1

RETURN_USER:
    ldr r0, =USER_TEXT

    msr CPSR_c, #0x10               @ USER mode
    bx r0                           @ go!

@@@@@@@@@
@@@@@@@@@ handlers  
@@@@@@@@@

IRQ_HANDLER:
    stmfd sp!, {r0-r12, lr}

    mrs r0, SPSR                    @ save SPSR before syscall
    stmfd sp!, {r0}                 @ push into stack

    ldr r1, =GPT_BASE               @ r1 <= GPT_BASE
    mov r0, #1
    str r0, [r1, #GPT_SR]           @ interruption already happenned!

    ldr r1, =COUNTER                @ increments counter
    ldr r0, [r1]
    add r0, r0, #1
    str r0, [r1]

    mov r10, #0                     @ total_execution_functions <= 0
    ldr r9, =EXECUTION_P            @ save execution stack pointer

    ldr r12, =FUN_ENABLED
    ldr r1, [r12]

    cmp r1, #0                      @ if FUN_ENABLED == 0
    beq END_IRQ                     @   go home

@@ ALARMS CHECK @@
alarms_check:
    ldr r1, =ALARMS_SIZE            @ load into memory alarms size
    ldr r1, [r1]

    cmp r1, #0                      @ do we have a list to check?
    beq callbacks_check             @ nope, go home

    @ set things up...
    mov r5, #0                      @ yes, sir

    ldr r2, =ALARMS_P
    ldr r3, =ALARMS_TIME

    alarms_handle:
        ldr r4, [r3, r5, lsl #BYTE]     @ get alarm time

        cmp r4, r0                      @ if ALARMS_TIME > COUNTER
        bgt next_alarm                  @   go to next alarm
                                        @ else
        ldr r8, [r2, r5, lsl #BYTE]     @   save pointed function

        str r8, [r9, r10, lsl #BYTE]    @ save pointer to stack
        add r10, r10, #1                @ total_execution++

        mov r8, #0                      @ set FUN_ENABLED off
        str r8, [r12]

        mov r8, r5                      @ r0 <= r5
                                        @ TMP_VARIABLE so we dont modify r5

        delete_alarm:
            add r6, r8, #1              @ go to next available 

            cmp r1, r6                  @ is it a valid memory?
            bls delete_alarm_end        @   go away!
                                        @ else
                                        @   copy it to previous memory!

            ldr r7, [r2, r6, lsl #BYTE]
            str r7, [r2, r0, lsl #BYTE]

            ldr r7, [r3, r6, lsl #BYTE]
            str r7, [r3, r0, lsl #BYTE]

            add r8, r8, #1              @ r0++
            b delete_alarm

        delete_alarm_end:
            sub r1, r1, #1              @ r1--
            sub r5, r5, #1              @ r5--

        next_alarm:
            add r5, #1                  @ r5++

            cmp r5, r1                  @ are we done yet?
            blt alarms_handle           @ nope, go to work!

        ldr r5, =ALARMS_SIZE            @ save new size
        str r1, [r5]

@@ CALLBACKS CHECK @@
callbacks_check:
    ldr r1, =CALLBACKS_SIZE         @ load into memory
    ldr r1, [r1]

    cmp r1, #0                      @ do we have a list to check?
    beq execution_check             @ nope, go home

    @ set things up...
    mov r5, #0                      @ yes, sir

    ldr r2, =CALLBACKS_P
    ldr r3, =CALLBACKS_DIST
    ldr r4, =CALLBACKS_ID

    callbacks_handle:
        ldr r0, [r4, r5, lsl #BYTE]     @ read sonar id

        mov r7, #16
        svc 0x0

        ldr r6, [r3, r5, lsl #BYTE]     @ read min_dist

        cmp r0, r6                      @ if r0 > min_dist
        bhi next_callback               @   go to next callback
                                        @ else
        ldr r0, [r2, r5, lsl #BYTE]     @   go to pointed function

        str r0, [r9, r10, lsl #BYTE]    @ save pointer to stack
        add r10, r10, #1                @ total_execution++

        mov r8, #0                      @ set FUN_ENABLED off
        str r8, [r12]

        next_callback:
            add r5, #1              @ r5++

            cmp r5, r1              @ are we done yet?
            blt callbacks_handle    @ nope, go to work!

execution_check:                    @ execute all pointers in the stack
    cmp r10, #0                     @ do we have functions to execute?
    beq END_IRQ                     @   nope, go home!
                                    @ else
                                    @   execute all of them!
    mov r1, #0                      @ counter
    msr CPSR_c, #0x10               @ user mode IRQ enabled

    execute_fun:
        ldr r0, [r9, r1, lsl #BYTE]

        stmfd sp!, {r0-r3, r12, lr}
        blx r0                      @ go to function
        ldmfd sp!, {r0-r3, r12, lr}

        add r1, r1, #1

        cmp r10, r1
        bhi execute_fun             @ while r10 > r1

    ldr r12, =FUN_ENABLED
    mov r1, #1                      @ enable callbacks or alarms
    str r1, [r12]

    ldr r1, =SPV_ENABLED            @ enable SPV syscall
    mov r0, #1
    str r0, [r1]

    mov r7, #66                     @ change back to supervisor mode
    svc 0x0

    ldr r0, =SPV_ENABLED            @ disable SPV syscall
    mov r1, #0                      @ r1 <= 0
    str r1, [r0]

    msr CPSR_c, #0x12               @ and change back to IRQ mode

END_IRQ:
    ldmfd sp!, {r0}                 @ pop from stack!
    msr SPSR, r0                    @ save previous SPSR

    ldmfd sp!, {r0-r12, lr}         @ save used registers

    subs lr, lr, #4                 @ ajust pc to the correct value 
    movs pc, lr                     @ return

@ go to specified syscall
SYSCALL_HANDLER:
    stmfd sp!, {r10}                @ save register
    mrs r10, SPSR                   @ save SPSR before syscall

    msr CPSR_c, #0x93

    msr SPSR, r10                   @ save previous SPSR
    ldmfd sp!, {r10}                @ pop from stack!

    cmp r7, #16
    beq read_sonar

    cmp r7, #17
    beq register_proximity_callback

    cmp r7, #18
    beq set_motor_speed

    cmp r7, #19
    beq set_motors_speed

    cmp r7, #20
    beq get_time

    cmp r7, #21
    beq set_time

    cmp r7, #22
    beq set_alarm

    cmp r7, #66
    beq change_mode

    movs pc, lr

@@@@@@@@@
@@@@@@@@@ routines
@@@@@@@@@

read_sonar:
    stmfd sp!, {r1-r2, lr}              @ save registers

    stmfd sp!, {r10}                    @ save register
    mrs r10, SPSR                       @ save SPSR before syscall

    msr CPSR_c, #0x13

    msr SPSR, r10                       @ save previous SPSR
    ldmfd sp!, {r10}                    @ pop from stack!

    cmp r0, #MAX_SENSOR_ID
    bhi return_neg_one                  @ invalid id > MAX_SENSOR_ID

    and r0, #SONAR_ID_MASK              @ make sure to catch only the 
                                        @ last 4 bits

    ldr r1, =GPIO_BASE                  @ GPIO base values

    ldr r2, [r1, #GPIO_DR]              @ loads value from DR
    bic r2, r2, #(SONAR_ID_MASK << 2)   @ set SONAR_MUX to zero

    lsl r0, #2                          @ set bits from id

    orr r2, r2, r0                      @ load it to DR

    bic r2, r2, #(1 << 1)               @ TRIGGER <= 0
    str r2, [r1, #GPIO_DR]              @ save it!

    mov r0, #10                         
    bl wait                             @ wait 10 ms

    ldr r2, [r1, #GPIO_DR]
    orr r2, r2, #(1 << 1)               @ TRIGGER <= 1
    str r2, [r1, #GPIO_DR]

    mov r0, #15                         
    bl wait                             @ wait 15 ms

    ldr r2, [r1, #GPIO_DR]
    bic r2, r2, #(1 << 1)               @ TRIGGER <= 0
    str r2, [r1, #GPIO_DR]

    wait_for_flag:
        mov r0, #10                         
        bl wait                         @ wait 10 ms

        ldr r2, [r1, #GPIO_DR]
        and r2, r2, #1

        cmp r2, #1
        bne wait_for_flag               @ while (flag != 1)

    ldr r2, [r1, #GPIO_DR]              @ r0 <= SONAR_DATA
    mov r0, r2, lsr #6

    ldr r1, =SONAR_DATA_MASK
    and r0, r1                          @ apply mask to obtain last 12 bits

    ldmfd sp!, {r1-r2, lr}              @ recover registers

    movs pc, lr                         @ return!

register_proximity_callback:
    stmfd sp!, {r4-r7, lr}                  @ save registers

    @ make the necessary verifications
    ldr r4, =CALLBACKS_SIZE
    ldr r4, [r4]

    cmp r4, #MAX_CALLBACKS              @ if r1 >= MAX_CALLBACKS
    bcs return_neg_one                  @   return -1

    cmp r0, #MAX_SENSOR_ID              @ if id > MAX_SENSOR_ID
    bhi return_neg_two                  @   return -2

    and r0, #SONAR_ID_MASK              @ catch only the last 4 bits of id

    @ everything is in order, add to stack!
    ldr r5, =CALLBACKS_ID
    ldr r6, =CALLBACKS_DIST
    ldr r7, =CALLBACKS_P

    str r0, [r5, r4, lsl #BYTE]
    str r1, [r6, r4, lsl #BYTE]
    str r2, [r7, r4, lsl #BYTE]

    add r4, r4, #1

    ldr r5, =CALLBACKS_SIZE             @ save new size
    str r4, [r5]

    ldmfd sp!, {r4-r7, lr}                  @ recover registers
    
    b return_success

set_motor_speed:
    stmfd sp!, {r1-r3, lr}                  @ save registers

    cmp r1, #MAX_SPEED                  @ if r1 > MAX_SPEED
    bhi return_neg_two                  @   invalid, number is too big!

    @ set speed value!
    and r1, #SPEED_VALUE_MASK           @ keep the last 6 bits of the number

    ldr r2, =GPIO_BASE                  @ load value from DR
    ldr r3, [r2, #GPIO_DR]

    @ compare r0 with corresponding id of the motor
    cmp r0, #0                          @ if r0 == 0
    beq set_motor_0                     @   motor_0

    cmp r0, #1                          @ else if r0 == 1
    beq set_motor_1                     @   motor_1

    b return_neg_one                    @ else
                                        @   invalid

    set_motor_0:
        mov r1, r1, lsl #19             @ shift the velocity to match 
                                        @ the corresponding motor

        and r3, #MOTOR_0_MASK           @ set to zero only the motor0 bits

        b write_speed

    set_motor_1:
        mov r1, r1, lsl #26             @ shifts the velocity to match 
                                        @ the corresponding motor

        and r3, #MOTOR_1_MASK           @ sets to zero only the motor1 bits

    write_speed:
        orr r3, r1                      @ set motor bits accordingly

        str r3, [r2, #GPIO_DR]          @ store value back to DR

    ldmfd sp!, {r1-r3, lr}                  @ recover registers

    b return_success

set_motors_speed:
    stmfd sp!, {r1-r3, lr}                  @ save registers

    cmp r0, #MAX_SPEED                  @ if r0 > MAX_SPEED
    bhi return_neg_two                  @   invalid, number is too big!

    cmp r1, #MAX_SPEED                  @ if r1 > MAX_SPEED
    bhi return_neg_two                  @   invalid, number is too big!

    and r0, #SPEED_VALUE_MASK           @ applies mask to both speed values
    and r1, #SPEED_VALUE_MASK           @ obtaining last 6 bits

    mov r0, r0, lsl #19                 @ shifts bits to match corresponding motor
    mov r1, r1, lsl #26

    ldr r2, =GPIO_BASE                  @ loads value from DR
    ldr r3, [r2, #GPIO_DR]

    and r3, #MOTOR_0_MASK               @ sets to zero motor 0 and 1 bits
    and r3, #MOTOR_1_MASK

    orr r3, r0                          @ copy values
    orr r3, r1

    str r3, [r2, #GPIO_DR]              @ store new speed values!

    ldmfd sp!, {r1-r3, lr}                  @ recover registers


    b return_success

get_time:
    stmfd sp!, {lr}

    ldr r0, =COUNTER
    ldr r0, [r0]

    ldmfd sp!, {lr}

    movs pc, lr                         @ return counter

set_time:
    stmfd sp!, {r1, lr} 

    ldr r1, =COUNTER
    str r0, [r1]

    ldmfd sp!, {r1, lr}

    movs pc, lr                         @ simply return

set_alarm:
    stmfd sp!, {r3-r7, lr}                  @ save registers

    @ make the necessary verifications
    ldr r6, =ALARMS_SIZE
    ldr r3, [r6]

    cmp r3, #MAX_ALARMS                 @ if r1 >= MAX_ALARMS
    bge return_neg_one                  @   return -1

    ldr r4, =COUNTER
    ldr r4, [r4]

    cmp r4, r1                          @ if COUNTER > desired_time
    bhi return_neg_two                  @   return -2

    @ everything is in order, add to stack!
    ldr r4, =ALARMS_P
    ldr r5, =ALARMS_TIME

    str r0, [r4, r3, lsl #BYTE]
    str r1, [r5, r3, lsl #BYTE]

    add r3, r3, #1

    str r3, [r6]        

    ldmfd sp!, {r3-r7, lr}                  @ clean up the mess!

    b return_success

change_mode:                            @ switch to supervisor mode
    stmfd sp!, {r0}

    ldr r0, =SPV_ENABLED
    ldr r0, [r0]

    cmp r0, #0                          @ if SPV_ENABLED == 1

    ldmfd sp!, {r0}

    bne exit_as_spv                     @   exit as spv
    
    movs pc, lr                         @ change back to user mode

    exit_as_spv:
        mov pc, lr

@@@@@@@@@ helper functions
wait:                                   @ wait for r0 in ms
    stmfd sp!, {r1-r3, lr}

    ldr r2, =CLOCK_WAIT
    mul r1, r2, r0                      @ aimed value
    
    count:
        sub r1, r1, #3

        cmp r1, #0                      @ while: r1 > r2
        bgt count

    ldmfd sp!, {r1-r3, pc} 

@@@@@@@@@ return options
return_success:                         @ return 0
    mov r0, #0
    movs pc, lr

return_neg_one:                         @ return -1
    mov r0, #-1
    movs pc, lr

return_neg_two:                         @ return -2
    mov r0, #-2
    movs pc, lr

@@@@@@@@@
@@@@@@@@@ data  
@@@@@@@@@
.data
    COUNTER:        .word 0

    CALLBACKS_SIZE: .word 0
    CALLBACKS_P:    .skip MAX_CALLBACKS * 32
    CALLBACKS_DIST: .skip MAX_CALLBACKS * 32
    CALLBACKS_ID:   .skip MAX_CALLBACKS * 32

    FUN_ENABLED:  .word 0

    ALARMS_SIZE:    .word 0
    ALARMS_P:       .skip MAX_ALARMS * 32
    ALARMS_TIME:    .skip MAX_ALARMS * 32

    EXECUTION_P:    .skip MAX_EXECUTION * 32

    STACK_P:        .skip STACK_SIZE * 4

    SPV_ENABLED:    .word 0
