.text				
	.align 4		
	.globl main	
	.globl _start

_start:

main:
	mov r0, #1	
					
	ldr r1, =string 
	mov r2, #19		
					
	mov r7, #4		
	svc 0x0			
	mov r7, #1		
					
	svc 0x0

.data				

string:  .asciz "Isadora - ra158018\n"  @coloca a string na memoria
