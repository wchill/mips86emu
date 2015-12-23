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

inline void MIPS32_Cpu::write_reg(int reg, uint32_t word) {
    // Bitwise ANDing with ~((reg != 0) - 1) means all writes to $0 will be 0
    registers[reg] = word & ~((reg != 0) - 1);
}

inline void MIPS32_Cpu::write_reg(int reg, int32_t word) {
    // Bitwise ANDing with ~((reg != 0) - 1) means all writes to $0 will be 0
    registers[reg] = static_cast<uint32_t>(word) & ~((reg != 0) - 1);
}

inline int32_t MIPS32_Cpu::read_reg_signed(int reg) {
    return static_cast<int32_t>(registers[reg]);
}

inline uint32_t MIPS32_Cpu::read_reg_unsigned(int reg) {
    return registers[reg];
}

void MIPS32_Cpu::parse_instruction(uint32_t instruction, r_inst *r_params, i_inst *i_params, j_inst *j_params) {
    r_params->instruction = instruction;
    r_params->opcode = (uint8_t) ((instruction >> 26) & 0b111111);
    r_params->rs = (uint8_t) ((instruction >> 21) & 0b11111);
    r_params->rt = (uint8_t) ((instruction >> 16) & 0b11111);
    r_params->rd = (uint8_t) ((instruction >> 11) & 0b11111);
    r_params->shamt = (uint8_t) ((instruction >> 6) & 0b11111);
    r_params->funct = (uint8_t) (instruction & 0b111111);

    i_params->instruction = instruction;
    i_params->opcode = r_params->opcode;
    i_params->rs = r_params->rs;
    i_params->rt = r_params->rt;
    i_params->immediate = (uint16_t) (instruction & 0xffff);
    i_params->sign_ext_imm = (uint32_t) (0xFFFF0000 * (i_params->immediate >> 15) + i_params->immediate);

    j_params->instruction = instruction;
    j_params->opcode = r_params->opcode;
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
    pc = MEMORY_TEXT + 24;
}

void MIPS32_Cpu::add_memory_mapped_device(std::shared_ptr<MemoryMappedDevice> device) {
    // std::cout << "Adding memory mapped device " << device.get_device_name() << std::endl;
    memory_mapped_devices.push_back(std::move(device));
}

SharedMemory MIPS32_Cpu::get_cpu_memory() {
    return memory;
}

void MIPS32_Cpu::tick() {
    // big endian
    uint32_t instruction = __bswap_32(memory->read_word(pc));

    uint8_t opcode = (uint8_t) (instruction >> 26);

    r_inst r_params;
    i_inst i_params;
    j_inst j_params;

    //printf("Executing instruction 0x%08x at 0x%08x\n", instruction, pc);
    switch(opcode) {
        case OP_OTHER0:
            switch(r_params.funct) {
                #include "instructions/arithmetic.h"
                default:
                    cerr << "Unimplemented opcode";
            }
            break;
        case OP_SPECIAL2:
            break;
        case OP_J:
            break;
        case OP_JAL:
            break;
        // I-type
    }
    //pc += 4 * !branch_taken;
}