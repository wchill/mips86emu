#include "MIPS32_Cpu.h"

// TODO: Support little endian memory access
MIPS32_Cpu::MIPS32_Cpu(bool little_endian) : little_endian(little_endian) {
    memory = std::make_shared<Memory>();
    coprocessors[0] = new MIPS32_Coprocessor0(memory);
    coprocessors[1] = new MIPS32_Coprocessor1(memory);
    coprocessors[2] = NULL;
    coprocessors[3] = NULL;
    reset();
}

MIPS32_Cpu::~MIPS32_Cpu() {
    reset();
    memory_mapped_devices.clear();
}

void MIPS32_Cpu::reset() {
    for(auto it = memory_mapped_devices.begin(); it != memory_mapped_devices.end(); ++it) {
        (*it)->reset();
    }
    for (int i = 0; i < 4; ++i) {
        if (coprocessors[i] != NULL) {
            coprocessors[i]->reset();
        }
    }
    memory->clear_memory();
    std::fill(registers, registers + NUM_REGISTERS, 0);
    write_reg(R_SP, STACK_TOP);
    write_reg(R_GP, DYNAMIC_BOTTOM);
    // start of text segment
    // temporarily set to 0x400018 until I figure out the ELF format
    pc = MEMORY_TEXT + 20;
}

void MIPS32_Cpu::add_memory_mapped_device(std::shared_ptr<MemoryMappedDevice> device) {
    // std::cout << "Adding memory mapped device " << device.get_device_name() << std::endl;
    memory_mapped_devices.push_back(std::move(device));
}

SharedMemory MIPS32_Cpu::get_cpu_memory() {
    return memory;
}

bool MIPS32_Cpu::tick() {
    try {
        last_pc = pc;
        execute(get_next_instruction());
    } catch(const CpuException &e) {
        cerr << e.what() << endl;
        //cerr << "Press Enter to continue..." << endl;
        //cin.get();
        pc = e.pc;
    } catch(const std::runtime_error &e) {
        cerr << e.what() << endl;
        //cerr << "Press Enter to continue..." << endl;
        //cin.get();
        pc = (last_pc & 0xfffffffc) + 4;
    }
    return true;
}

void MIPS32_Cpu::execute(uint32_t instruction) {
    cout << fmt::sprintf("  - Executing instruction @ %#08x: %#08x", pc, instruction) << endl;
    Instruction params(instruction, pc);
    switch (params.opcode) {
        case OP_OTHER0:
            switch (params.funct) {
                #include "instructions/arithmetic.h"
                #include "instructions/shift.h"
                #include "instructions/logical.h"
                #include "instructions/jump_reg.h"
                #include "instructions/syscall.h"
                default:
                    cerr << fmt::sprintf("Unimplemented funct @ %#08x: %#02x", pc, params.funct) << endl;
            }
            break;

        #include "instructions/immutable.h"
        #include "instructions/loadstore.h"
        #include "instructions/branch.h"
        #include "instructions/jump.h"
        #include "instructions/coprocessor.h"
        #include "instructions/branch_other.h"
        #include "instructions/special2.h"

        default:
            cerr << fmt::sprintf("Unimplemented opcode @ %#08x: %#02x", pc, params.opcode) << endl;
    }
}