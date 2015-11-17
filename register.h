#ifndef MIPS_REGISTERS
#define MIPS_REGISTERS

#include "mips.h"

uint32_t mips_registers[32];
uint32_t mips_register_hi;
uint32_t mips_register_lo;

static inline void write_reg(uint8_t reg, uint32_t word) {
    if(reg != R_ZERO) {
        mips_registers[reg] = word;
    }
}

static inline void write_hi_reg(uint32_t word) {
    mips_register_hi = word;
}

static inline void write_lo_reg(uint32_t word) {
    mips_register_lo = word;
}

static inline uint32_t read_reg_unsigned(uint8_t reg) {
    return (uint32_t) mips_registers[reg];
}

static inline int32_t read_reg_signed(uint8_t reg) {
    return (int32_t) mips_registers[reg];
}

static inline uint32_t read_hi_reg() {
    return (uint32_t) mips_register_hi;
}

static inline uint32_t read_lo_reg() {
    return (uint32_t) mips_register_lo;
}

#endif
