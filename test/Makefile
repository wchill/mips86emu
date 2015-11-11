all: helloworld.hex

clean:
	rm -rf *.o *.hex

helloworld.hex: helloworld.s
	mips-none-elf-gcc helloworld.s -Os -nostdlib --entry main -o helloworld
	mips-none-elf-objcopy -O binary helloworld helloworld.hex
	rm helloworld
