@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @
@                                                                             @
@               	      Robot API implementation                            @
@   Author: Isadora Sophia Garcia Rodopoulos                                  @
@   RA: 158018                                                                @
@                                                                             @
@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @

	@ Global symbol
	.global set_speed_motor, set_speed_motors, read_sonar, read_sonars

	.align 4
@ Sets motor speed. 
@ Parameters:
@   speed: the motor speed (only the last 6 bits are used)
@	id: the motor id (0 for left motor, 1 for right motor)
set_speed_motor:
	stmfd sp!, {r7, lr}	@ Save the callee-save registers     
		                @ and the return address.
	cmp r1, #0			@ identify the id
	bgt right			@ if id is 1, go to right motor

	mov r7, #126		@ left motor
	b call

	right:				@ right motor
		mov r7, #127

	call:
		svc 0x0		    @ make the syscall

	ldmfd sp!, {r7, pc} @ Restore the registers and return


@ Sets both motors speed.
@ Parameters:
@   spd_m0: the speed of motor 0 (Only the last 6 bits are used)
@   spd_m1: the speed of motor 1 (Only the last 6 bits are used)
set_speed_motors:
	stmfd sp!, {r7, lr}	@ Save the callee-save registers     
		                @ and the return address.
	mov r7, #124
	svc 0x0		    	@ make the syscall

	ldmfd sp!, {r7, pc} @ Restore the registers and return


@ Reads one of the sonars.
@ Parameters:
@  	id: the sonar id (ranges from 0 to 15).
read_sonar:
	stmfd sp!, {r7, lr}	@ Save the callee-save registers     
		                @ and the return address.
	mov r7, #125
	svc 0x0		    	@ make the syscall

	ldmfd sp!, {r7, pc} @ Restore the registers and return


@ Reads all sonars at once.
@ Parameters:
@  	sonars: array of 16 unsigned integers. The distances are stored
@   on the array.
read_sonars:
	stmfd sp!, {r1-r2, r7, lr}	@ save the callee-save registers
		                		@ and the return address.
    mov r1, r0					@ save array address
    mov r2, #0					@ counter

    loop:
		mov r7, #125
		svc 0x0		   			@ make the syscall

		str r0, [r1]			@ assign sonar value to the array
		add r1, r1, #1			@ +1 the address

		add r2, r2, #1
		cmp r2, #16
		blt loop				@ while r2 < 16

	ldmfd sp!, {r1-r2, r7, pc} 	@ restore the registers and return
