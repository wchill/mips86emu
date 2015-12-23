#include <MIPS32_Cpu.h>
#include <iostream>
#include "MIPS32_Cpu.h"

// TODO: Support little endian memory access
MIPS32_Cpu::MIPS32_Cpu(bool little_endian) : little_endian(little_endian) {
    memory = std::make_shared<Memory>();
    reset();
}

MIPS32_Cpu::~MIPS32_Cpu() {
    reset();
    memory_mapped_devices.clear();
}

inline inst_params MIPS32_Cpu::parse_instruction(uint32_t instruction) {
    inst_params params;
    params.instruction = instruction;
    params.opcode = static_cast<uint8_t>((instruction >> 26) & 0b111111);
    params.rs = static_cast<uint8_t>((instruction >> 21) & 0b11111);
    params.rt = static_cast<uint8_t>((instruction >> 16) & 0b11111);
    params.rd = static_cast<uint8_t>((instruction >> 11) & 0b11111);
    params.shamt = static_cast<uint8_t>((instruction >> 6) & 0b11111);
    params.funct = static_cast<uint8_t>(instruction & 0b111111);
    params.immediate = static_cast<uint16_t>(instruction & 0xffff);
    params.signed_imm = static_cast<int16_t>(params.immediate);
    params.sign_ext_imm = static_cast<uint32_t>((0xFFFF0000 & ~((params.immediate >> 15 != 0) - 1)) + params.immediate);
    params.signed_sign_ext_imm = static_cast<int32_t>(params.sign_ext_imm);
    params.target = (pc & 0xf0000000) | ((instruction & 0x3ffffff) << 2);
    return params;
}

void MIPS32_Cpu::reset() {
    for(auto it = memory_mapped_devices.begin(); it != memory_mapped_devices.end(); ++it) {
        (*it)->reset();
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

void MIPS32_Cpu::tick() {
    execute(get_next_instruction());
}

void MIPS32_Cpu::execute(uint32_t instruction) {
    cout << fmt::sprintf("  - Executing instruction @ %#08x: %#08x", pc, instruction) << endl;
    if (instruction != 0) {
        inst_params params = parse_instruction(instruction);
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
#include "instructions/branch_other.h"
            case OP_SPECIAL2:
                break;
            default:
                cerr << fmt::sprintf("Unimplemented opcode @ %#08x: %#02x", pc, params.opcode) << endl;
                // I-type
        }
        //pc += 4 * !branch_taken;
    }
}