#include "MIPS32_Coprocessor0.h"

MIPS32_Coprocessor0::MIPS32_Coprocessor0(SharedMemory memory) : MIPS32_Coprocessor(memory) {
    registers = new uint32_t[32];
    control_reg = new uint32_t[32];
    num_registers = 32;
    num_control = 32;
}

void MIPS32_Coprocessor0::load_word(Instruction params, uint32_t addr) {
    throw std::runtime_error("lwc0 is not a valid instruction");
}

void MIPS32_Coprocessor0::store_word(Instruction params, uint32_t addr) {
    throw std::runtime_error("swc0 is not a valid instruction");
}

void MIPS32_Coprocessor0::write_reg(uint8_t reg, uint32_t word) {
    if (num_registers < reg) {
        throw std::runtime_error(fmt::sprintf("Attempted to write invalid register %d on cop0", reg));
    }
    registers[reg] = word;
}

uint32_t MIPS32_Coprocessor0::read_reg(uint8_t reg) {
    if (num_registers < reg) {
        throw std::runtime_error(fmt::sprintf("Attempted to read invalid register %d on cop0", reg));
    }
    return registers[reg];
}

void MIPS32_Coprocessor0::write_control(uint8_t reg, uint32_t word) {
    if (num_control < reg) {
        throw std::runtime_error(fmt::sprintf("Attempted to write invalid control register %d on cop0", reg));
    }
    control_reg[reg] = word;
}

uint32_t MIPS32_Coprocessor0::read_control(uint8_t reg) {
    if (num_control < reg) {
        throw std::runtime_error(fmt::sprintf("Attempted to read invalid control register %d on cop0", reg));
    }
    return control_reg[reg];
}