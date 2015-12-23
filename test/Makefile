all: helloworld.hex torture_core.hex divide.hex

clean:
	rm -rf *.o *.hex

helloworld.hex: helloworld.s
	mips-none-elf-gcc helloworld.s -O0 -nostdlib -fno-delayed-branch --entry main -o helloworld
	mips-none-elf-objdump -d helloworld
	mips-none-elf-objcopy -O binary helloworld helloworld.hex
	rm helloworld

torture_core.hex: torture_core.s
	mips-none-elf-gcc torture_core.s -O0 -nostdlib -mips32 -fno-delayed-branch --entry main -o torture_core
	mips-none-elf-objdump -d torture_core > output.txt
	mips-none-elf-objcopy -O binary torture_core torture_core.hex
	rm torture_core

divide.hex: divide.s
	mips-none-elf-gcc divide.s -O0 -nostdlib -mips32 -fno-delayed-branch --entry main -o divide
	mips-none-elf-objdump -d divide > divide.txt
	mips-none-elf-objcopy -O binary divide divide.hex
	rm divide
