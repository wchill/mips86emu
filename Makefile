all: bin/mips86emu

clean:
	rm -rf bin

bin/mips86emu:
	mkdir -p bin
	gcc emulator.c memory.c instructions.c -g -O3 -Wall -o bin/mips86emu

emulator.o: emulator.c
	gcc emulator.c -g -O3 -Wall

mips.o: mips.h
	gcc mips.h -g -O3 -Wall

register.o: register.h
	gcc register.h -g -O3 -Wall

memory.o: memory.c memory.h
	gcc memory.c -g -O3 -Wall

instructions.o: instructions.c instructions.h
	gcc instructions.c -g -O3 -Wall
