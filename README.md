#mips86emu

A MIPS emulator written in C (soon-to-be-C++) implementing the MIPS32 ISA.

The goal is to have a fully working emulator with dynamic recompilation to x86 assembly (hence the name), memory mapped I/O for graphics/sound/networking, and the ability to have a full Linux kernel running in the emulator.

Many of the most useful instructions have already been implemented, along with a minimal implementation for coprocessors 0 and 1 (which currently do not provide any additional functionality).

##TODO:
* Reimplement in C++ for modularity/maintainability reasons
* Finish implementing opcodes
* Finish implementing coprocessors
* Finish implementing syscalls for bare metal assembly code
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
