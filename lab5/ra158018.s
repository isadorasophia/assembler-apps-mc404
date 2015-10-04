@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @
@                                                                             @
@                               Laboratorio 5                                 @
@    Autora: Isadora Sophia Garcia Rodopoulos                                 @
@    RA: 158018                                                               @
@    Descricao: Recebe um numero e retorna o seu valor na sequencia de        @
@               Fibonacci, em binario.                                        @
@    ex input:  0000000000000010                                              @
@       output: 0000000000000001                                              @
@                                                                             @
@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @

.text
.align 4
.globl _start                 @ torna o rotulo _start global, ou seja, visivel pelo ligador.

_start:                       @ ponto de entrada do programa.
	
@@@@@@@@@ Le uma string binaria e converte para um valor em r0 @@@@@@@@@@@@@@
        
        mov r0, #0            @ carrega o valor 0 em r0, indicando que a entrada da
                              @ syscall read sera em stdin
        ldr r1, =string       @ carrega em r1 o endereco de um buffer na memoria.
        mov r2, #17           @ carrega em r2 a quantidade maxima de dados (em bytes) a se lida.
                              @ r0, r1 e r2 serao os argumentos da syscall read
        mov r7, #3            @ carrega o valor 3 para r7, indicando o tipo da syscall (read)
        svc 0x0               @ realiza uma chamada de sistema (syscall)

	@ O sistema se encarregara de ler 16 bytes da entrada padrao e gravar no buffer indicado em r1.
	
        mov  r0, #0           @ para armazenar valor convertido
        ldr r1, =string       @ carrega em r1 o endereco da string
        mov  r2, #0           @ iterador de loop

loop_c:
        mov  r0, r0, LSL #1   @ desloca o valor de r0 para esquerda 
                              @ LSB (bit menos significativo) de r0 recebe 0 
        ldrb r3, [r1, r2]     @ r3 = string[r2]
        cmp  r3, #49          @ compara r3 com '1'  (49 em ASCII)
        blt  reset            @ se for menor: r0 ja esta com 0 no LSB
        orr  r0, r0, #1       @ senao: LSB de r0 recebe 1

reset:
        add r2, r2, #1        @ incrementa variavel de controle da iteracao
        cmp r2, #16           @ compara r2 com o tamanho da string binaria
        blt loop_c            @ se ainda esta com um valor menor que o tamanho da string


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


        mov r6, #0            @ r6 representa o contador de iteracoes

    @ r7 e r8 correspondem, respectivamente, ao penultimo e ultimo valor da sequencia.

        mov r7, #0            @ valores iniciais de fibonacci
        mov r8, #1

loop_f:
        mov r9, r8            @ guarda o valor do ultimo numero em aux (r9)
        add r8, r7, r8        @ soma dos dois ultimos numeros da sequencia
        mov r7, r9            @ r7 recebe o penultimo valor

        add r6, #1            @ r6++
        cmp r6, r0            @ r6 < r0?
        blt loop_f            @ then: itera pelo no. de vezes necessario

        mov r0, r7            @ r0 = penultimo valor


@@@@@@@@@@@@@@@@@@@@@@@ Valor de r0 para string binaria @@@@@@@@@@@@@@ 
 

        ldr r1, =string       @ string  para preencher com string binaria
        mov r2, #15           @ contador de iteracao
        mov r3, #1            @ valor para shift

loop:
        mov r4, #49           @ r4 = '1'    
        and r5, r0, r3        @ r5 = r0 AND r3
        cmp r5, #0            @ compara r5 com 0 (zero)
        bgt set               @ se for maior: r4 ja esta com '1'
        mov r4, #48           @ senao: r4 = '0'
set:
        strb r4, [r1, r2]     @ string[r2] = r4
        sub  r2, r2, #1       @ decrementa a variavel de controle do laco
        mov  r3, r3, LSL #1   @ r3 = r3 << 1
        cmp  r2, #0           @ compara r com zero
        bge  loop             @ se ainda e maior ou igual, volta para o loop


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@ imprime a string binaria

        mov r0, #1            @ carrega o valor 1 em r0, indicando que a saida da
                              @ syscall write sera em stdout
        ldr r1, =string       @ carrega em r1 o endereco da string
        mov r2, #17           @ carrega em r2 o tamanho da string. r0,r1 e r2 serao
                              @ os argumentos da syscall write
        mov r7, #4            @ carrega o valor 4 para r7, indica o tipo da syscall
        svc 0x0               @ realiza uma chamada de sistema (syscall)


        mov r7, #1            @ carrega o valor 1 em r7, indicando a escolha da
                              @ syscall exit
        svc 0x0         




.data

string:  .asciz "0000000000000000\n"  @coloca a string na memoria
