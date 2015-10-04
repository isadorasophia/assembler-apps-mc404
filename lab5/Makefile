FILENAME = ra158018

all: mkimage

%.o: %.s 
	arm-eabi-as $< -g -o $*.o

$(FILENAME): $(FILENAME).o
	arm-eabi-ld $< -g -o $(FILENAME) -Ttext=0x77802000 -Tdata=0x77802100

mkimage: $(FILENAME)
	mksd.sh --so dummylink --user $(FILENAME)

simulate_only: disk.img
	arm-sim --rom=dumboot --sd=disk.img

gdbtarget: disk.img
	arm-sim --rom=dumboot --sd=disk.img -g

gdbhost: $(FILENAME)
	arm-eabi-gdb $(FILENAME)

clean:
	rm -f $(FILENAME) $(FILENAME).o 