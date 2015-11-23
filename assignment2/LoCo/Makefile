MAINFILE = ronda
API = api_robot2
OUTPUT = robot_ctrl

all: mkimage

%.s: %.c 
	arm-eabi-gcc $< -S -o $*.s

%.o: %.s
	arm-eabi-as $< -o $*.o

$(OUTPUT): $(MAINFILE).o $(API).o
	arm-eabi-ld $(MAINFILE).o $(API).o -o $(OUTPUT) -Ttext=0x77802000

mkimage: $(OUTPUT)
	mksd.sh --so /home/bonnibel/graduation/mc404/simulador/simulador_player/bin/knrl --user $(OUTPUT)

simulate_target: disk.img
	player /home/bonnibel/graduation/mc404/simulador/simulador_player/worlds_mc404/simple.cfg

simulate_host: disk.img
	armsim_player --rom=/home/bonnibel/graduation/mc404/simulador/dumboot.bin --sd=disk.img

clean:
	rm -f $(OUTPUT) *.o $(MAINFILE).s