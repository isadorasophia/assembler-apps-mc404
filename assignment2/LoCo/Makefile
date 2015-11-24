MAINFILE = ronda
API = ../BiCo/api_robot2
SO = ../SOUL/SOUL
OUTPUT = robot_ctrl

BIN_PATH = /home/mc404/usr/bin
WORLDS_PATH = /home/mc404/usr/worlds_mc404

all: mkimage

%.s: %.c 
	arm-eabi-gcc $< -S -o $*.s

%.o: %.s
	arm-eabi-as $< -o $*.o

$(OUTPUT): $(MAINFILE).o $(API).o
	arm-eabi-ld $(MAINFILE).o $(API).o -o $(OUTPUT) -Ttext=0x77802000

mkimage: $(OUTPUT)
	mksd.sh --so $(SO) --user $(OUTPUT)

player_target: disk.img
	player $(WORLDS_PATH)/simple.cfg

player_host: disk.img
	armsim_player --rom=$(BIN_PATH)/dumboot.bin --sd=disk.img

gdb_target: disk.img
	arm-sim --rom=$(BIN_PATH)/dumboot.bin --sd=disk.img -gdb_target

gdb host: disk.img
	arm-eabi-gdb $(SO)

clean:
	rm -f $(OUTPUT) *.o $(MAINFILE).s disk.img
