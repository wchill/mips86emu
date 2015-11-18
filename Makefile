all: bin/mips86emu

clean:
	rm -rf bin

bin/mips86emu: emulator.c memory.c inst_r.c inst_i.c
	mkdir -p bin
	gcc emulator.c memory.c inst_r.c inst_i.c -g -O3 -Wall -o bin/mips86emu
