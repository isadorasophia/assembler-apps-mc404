DUMMYOS_PATH=/home/specg12-1/mc404/simulador/dummyos.elf
DUMBOOT_PATH=/home/specg12-1/mc404/simulador/dumboot.bin

FILENAME = teste

object: $(FILENAME).s 
	arm-eabi-as $< -g -o $(FILENAME).o

bin: $(FILENAME).o
	arm-eabi-ld $< -g -o $(FILENAME) -Ttext=0x77802000 -Tdata=0x77802050

image: $(FILENAME)
	mksd.sh --so $(DUMMYOS_PATH) --user $(FILENAME)

simulate: $(FILENAME)
	arm-sim --rom=$(DUMBOOT_PATH) --sd=disk.img -g

gdb: $(FILENAME)
	arm-eabi-gdb $(FILENAME)