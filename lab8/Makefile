# source set_path.sh
    
SO = ra158018
FILENAME = do_none

all: mkimage

%.o: %.s 
	arm-eabi-as $< -g -o $*.o

$(SO): $(SO).o
	arm-eabi-ld $< -o $(SO) -g --section-start=.iv=0x778005e0 -Ttext=0x77800700 -Tdata=0x77801800 -e 0x778005e0

$(FILENAME): $(FILENAME).o
    arm-eabi-ld $(FILENAME).o -o $(FILENAME) -g -Tdata=0x77801900 

mkimage: $(SO) $(FILENAME)
	mksd.sh --so $(SO) --user $(FILENAME)

gdbtarget: disk.img
	arm-sim --rom=dumboot --sd=disk.img -g

gdbhost: $(SO)
	arm-eabi-gdb $(SO)

clean:
	rm -f $(SO) $(FILENAME) *.o
