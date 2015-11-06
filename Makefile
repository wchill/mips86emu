all: bin/mips86emu

clean:
	rm -rf bin

bin/mips86emu: mips.c
	mkdir -p bin
	gcc mips.c -g -O3 -Wall -o bin/mips86emu
