#include "MIPS32_Coprocessor1.h"

MIPS32_Coprocessor1::MIPS32_Coprocessor1(SharedMemory memory) : MIPS32_Coprocessor(memory) {
    registers = new uint32_t[32];
    control_reg = new uint32_t[32];
    num_registers = 32;
    num_control = 32;
}

void MIPS32_Coprocessor1::load_word(Instruction params, uint32_t addr) {
    uint8_t reg = params.rt;
    if (num_control < reg) {
        throw std::runtime_error(fmt::sprintf("Attempted to load word into invalid register %d on cop1", reg));
    }
    write_reg(params.rt, memory->read_word(addr + params.sign_ext_imm));
}

void MIPS32_Coprocessor1::store_word(Instruction params, uint32_t addr) {
    uint8_t reg = params.rt;
    if (num_control < reg) {
        throw std::runtime_error(fmt::sprintf("Attempted to load word into invalid register %d on cop1", reg));
    }
    memory->write_word(addr + params.sign_ext_imm, read_reg(reg));
}

void MIPS32_Coprocessor1::write_reg(uint8_t reg, uint32_t word) {
    if (num_registers < reg) {
        throw std::runtime_error(fmt::sprintf("Attempted to write invalid register %d on cop1", reg));
    }
    registers[reg] = word;
}

uint32_t MIPS32_Coprocessor1::read_reg(uint8_t reg) {
    if (num_registers < reg) {
        throw std::runtime_error(fmt::sprintf("Attempted to read invalid register %d on cop1", reg));
    }
    return registers[reg];
}

void MIPS32_Coprocessor1::write_control(uint8_t reg, uint32_t word) {
    if (num_control < reg) {
        throw std::runtime_error(fmt::sprintf("Attempted to write invalid control register %d on cop1", reg));
    }
    control_reg[reg] = word;
}

uint32_t MIPS32_Coprocessor1::read_control(uint8_t reg) {
    if (num_control < reg) {
        throw std::runtime_error(fmt::sprintf("Attempted to read invalid control register %d on cop1", reg));
    }
    return control_reg[reg];
}