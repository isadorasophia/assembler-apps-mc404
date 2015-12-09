# ----------------------------------------
# Disciplina: MC404 - 1o semestre de 2015
# Professor: Edson Borin
#
# Descrição: Makefile para o segundo trabalho 
# ----------------------------------------

# ----------------------------------
# SOUL object files -- Add your SOUL object files here 
SOUL_OBJS=SOUL.o 

# ----------------------------------
# Compiling/Assembling/Linking Tools and flags
AS=arm-eabi-as
AS_FLAGS=-g

CC=arm-eabi-gcc
CC_FLAGS=-g

LD=arm-eabi-ld
LD_FLAGS=-g

# ----------------------------------
# Default rule
all: ronda

# ----------------------------------
# Generic Rules
%.o: %.s
	$(AS) $(AS_FLAGS) $< -o $@

%.o: %.c
	$(CC) $(CC_FLAGS) -c $< -o $@

# ----------------------------------
# Specific Rules
SOUL.x: $(SOUL_OBJS)
	$(LD) $^ -o $@ $(LD_FLAGS) --section-start=.iv=0x778005e0 -Ttext=0x77800700 -Tdata=0x77801000 -e 0x778005e0

RONDA.x: ronda.o bico.o
	$(LD) $^ -o $@ $(LD_FLAGS) -Ttext=0x77802a00

SEGUE-PAREDE.x: segue-parede.o bico.o
	$(LD) $^ -o $@ $(LD_FLAGS) -Ttext=0x77802a00

ronda: SOUL.x RONDA.x
	mksd.sh --so SOUL.x --user RONDA.x

segue-parede: SOUL.x SEGUE-PAREDE.x
	mksd.sh --so SOUL.x --user SEGUE-PAREDE.x

clean:
	rm -f *.x disk.img *.o
