#mips86emu

A MIPS emulator written in C targeting x86.

The goal is to have a fully working emulator with dynamic recompilation, memory mapped I/O for graphics/sound/networking, and the ability to have a full Linux kernel running in the emulator.

##TODO:
* Finish implementing opcodes
* Finish implementing syscalls for bare metal assembly code
* Refactor code to make it more modular
* Implement proper register/memory access
* Implement exceptions/interrupts
* Write a basic assembler
* Implement a just-in-time dynamic recompilation engine for faster execution
* Implement a debugger
* Implement terminal device
* Implement graphics device
* Implement disk access using emulated disk image
* Implement sound device
* Implement network adapter
* Cross-compile Linux to run on the emulator
* Write test cases
