# obs: path from my pc is /usr/lib/x86_64-linux-gnu
LIB_PATH = /usr/lib64

main.s: main.c
	gcc -S main.c -o main.s

math.s: math.c
	gcc -S math.c -o math.s

log.s: log.c
	gcc -S log.c -o log.s

main.o: main.s
	as main.s -o main.o

math.o: math.s
	as math.s -o math.o

log.o: log.s
	as log.s -o log.o

prog.x: main.o math.o log.o
	ld -dynamic-linker /lib64/ld-linux-x86-64.so.2 $(LIB_PATH)/crt1.o \
	$(LIB_PATH)/crti.o -L/usr/lib64 main.o math.o log.o -lc \
	$(LIB_PATH)/crtn.o -o prog.x

clean:
	rm -f main.s math.s log.s main.o math.o log.o prog.x
