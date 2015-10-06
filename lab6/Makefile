FILENAME = ra158018

all: mkimage

%.o: %.s 
	arm-eabi-as $< -g -o $*.o

$(FILENAME): $(FILENAME).o
	arm-eabi-ld $< -g -o $(FILENAME) -Ttext=0x77802000

mkimage: $(FILENAME)
	mksd.sh --so knrl --user $(FILENAME)

simulate_target: disk.img
	player /home/specg12-1/mc404/simulador/simulador_player/worlds_mc404/simple.cfg

simulate_host: disk.img
	armsim_player --rom=dumboot --sd=disk.img

gdbtarget: disk.img
	arm-sim --rom=dumboot --sd=disk.img -g

gdbhost: $(FILENAME)
	arm-eabi-gdb $(FILENAME)

clean:
	rm -f $(FILENAME) $(FILENAME).o 