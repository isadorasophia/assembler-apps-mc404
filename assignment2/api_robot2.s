@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @
@                                                                             @
@             Uoli Control Application Programming Implementation             @
@   Author: Isadora Sophia Garcia Rodopoulos                                  @
@   RA: 158018                                                                @
@                                                                             @
@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @

@ Global symbol
.global set_motor_speed, set_motors_speed, read_sonar, read_sonars
.global register_proximity_callback, add_alarm, get_time, set_time

.align 4
@@@@@@@@@
@@@@@@@@@ motors
@@@@@@@@@
@@@ Set motor speed.
@@@     r0:     the motor id (0 for left motor, 1 for right motor)
@@@     r1:     the motor speed (Only the last 6 bits are used)
@@@     return: void
set_motor_speed:
    stmfd sp!, {r0, r7, lr}     @ save the callee-save registers     
                                @ and the return address

    mov r7, #18                 @ left motor
    svc 0x0                     @ make the syscall

    ldmfd sp!, {r0, r7, pc}     @ Restore the registers and return

@@@ Set both motors speed.
@@@     r0:     the speed of motor 0 (Only the last 6 bits are used)
@@@     r1:     the speed of motor 1 (Only the last 6 bits are used)
@@@     return: void
set_motors_speed:
    stmfd sp!, {r0, r7, lr}     @ Save the callee-save registers     
                                @ and the return address.
    mov r7, #19
    svc 0x0                     @ make the syscall

    ldmfd sp!, {r0, r7, pc}     @ Restore the registers and return

@@@@@@@@@
@@@@@@@@@ sonars
@@@@@@@@@
@@@ Read one of the sonars
@@@     r0:     the sonar id (ranges from 0 to 15)
@@@     r1:     pointer to variable that will receive the distance.
@@@     return: void
read_sonar:
    stmfd sp!, {r0, r7, lr}     @ Save the callee-save registers     
                                @ and the return address.
    mov r7, #16
    svc 0x0                     @ make the syscall

    str r0, [r1]                @ save distance

    ldmfd sp!, {r0, r7, pc}     @ Restore the registers and 

@@@ Read all sonars at once
@@@     r0:     array of 16 unsigned integers. The distances are stored
@@@             on the array
@@@     return: void
read_sonars:
    stmfd sp!, {r0-r2, r7, lr}  @ save the callee-save registers
                                @ and the return address.

    .set TOTAL_SONARS, 15

    mov r1, r0                  @ save array address
    mov r2, #0                  @ counter

    loop:
        mov r0, r2              @ r0 <= current sonar id

        mov r7, #16
        svc 0x0                 @ make the syscall

        str r0, [r1, r2]        @ assign sonar value to the array

        add r2, r2, #1          @ r2++

        cmp r2, #TOTAL_SONARS   @ while r2 <= 15
        ble loop

    ldmfd sp!, {r0-r2, r7, pc}  @ restore the registers and return

@@@ Register a function f to be called whenever the robot gets close to an 
@@@ object. The user should provide the id of the sensor that must be monitored 
@@@ (sensor_id), a threshold distance (dist_threshold) and the user function 
@@@ that must be called. The system will register this information and monitor 
@@@ the sensor distance every DIST_INTERVAL cycles. Whenever the sensor
@@@ distance becomes smaller than the dist_threshold, the system calls 
@@@ the user function.
@@@     r0:     id of the sensor that must be monitored
@@@     r1:     threshold distance
@@@     r2:     address of the function that should be called when the robot
@@@             gets close to an object
@@@     return: void
register_proximity_callback:
    stmfd sp!, {r0, r7, lr}     @ save the callee-save registers
                                @ and the return address.

    mov r7, #17
    svc 0x0                     @ make the syscall

    ldmfd sp!, {r0, r7, pc}     @ restore the registers and return

@@@@@@@@@
@@@@@@@@@ timer
@@@@@@@@@
@@@ Add an alarm to the system.
@@@     r0:     function to be called when the alarm triggers
@@@     r1:     the time to invoke the alarm function
@@@     return: void
add_alarm:
    stmfd sp!, {r0, r7, lr}     @ save the callee-save registers
                                @ and the return address.

    mov r7, #22
    svc 0x0                     @ make the syscall

    ldmfd sp!, {r0, r7, pc}     @ restore the registers and return

@@@ Read the system time.
@@@     return: the system time
get_time:
    stmfd sp!, {r7, lr}         @ save the callee-save registers
                                @ and the return address.

    mov r7, #20
    svc 0x0                     @ make the syscall

    ldmfd sp!, {r7, pc}         @ restore the registers and return

@@@ Set the system time.
@@@     r0:     the new system time
@@@     return: void
set_time:
    stmfd sp!, {r7, lr}         @ save the callee-save registers
                                @ and the return address.

    mov r7, #21
    svc 0x0                     @ make the syscall

    ldmfd sp!, {r7, pc}         @ restore the registers and return
