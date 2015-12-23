#ifndef MIPS86CORE_MIPS32_CPU_H
#define MIPS86CORE_MIPS32_CPU_H

#include <cstdint>
#include <iostream>
#include <vector>
#include "MemoryMappedDevice.h"
#include "Memory.h"
#include "mips.h"

using std::cout;
using std::cerr;

// Registers
#define NUM_REGISTERS 32

#define R_ZERO  0   // Zero register
#define R_AT    1   // Assembler temporary
#define R_V0    2   // Function results/expression evaluation
#define R_V1    3
#define R_A0    4   // Arguments
#define R_A1    5
#define R_A2    6
#define R_A3    7
#define R_T0    8   // Temporaries
#define R_T1    9
#define R_T2    10
#define R_T3    11
#define R_T4    12
#define R_T5    13
#define R_T6    14
#define R_T7    15
#define R_S0    16  // Saved temporaries
#define R_S1    17
#define R_S2    18
#define R_S3    19
#define R_S4    20
#define R_S5    21
#define R_S6    22
#define R_S7    23
#define R_T8    24  // Temporaries
#define R_T9    25
#define R_K0    26  // Kernel
#define R_K1    27
#define R_GP    28  // Global pointer
#define R_SP    29  // Stack pointer
#define R_FP    30  // Frame pointer
#define R_RA    31  // Return address

typedef struct {
    uint32_t instruction;
    uint8_t opcode;
    uint8_t rs;
    uint8_t rt;
    uint8_t rd;
    uint8_t shamt;
    uint8_t funct;
} r_inst;

typedef struct {
    uint32_t instruction;
    uint8_t opcode;
    uint8_t rs;
    uint8_t rt;
    uint16_t immediate;
    uint32_t sign_ext_imm;
} i_inst;

typedef struct {
    uint32_t instruction;
    uint8_t opcode;
    uint32_t target;
} j_inst;

class MIPS32_Cpu {
private:
    bool little_endian = false;
    uint32_t pc;
    uint32_t registers[NUM_REGISTERS];
    SharedMemory memory;

    std::vector<std::shared_ptr<MemoryMappedDevice>> memory_mapped_devices;

    void write_reg(int reg, uint32_t word);
    void write_reg(int reg, int32_t word);
    int32_t read_reg_signed(int reg);
    uint32_t read_reg_unsigned(int reg);

    void parse_instruction(uint32_t instruction, r_inst *r_params, i_inst *i_params, j_inst *j_params);

    static inline uint16_t __bswap_16(uint16_t x) {
        return (x>>8) | (x<<8);
    }

    static inline uint32_t __bswap_32(uint32_t x) {
        return (__bswap_16(x&0xffff)<<16) | (__bswap_16(x>>16));
    }
public:
    MIPS32_Cpu(bool little_endian = false);
    ~MIPS32_Cpu();

    void reset();
    void add_memory_mapped_device(std::shared_ptr<MemoryMappedDevice> device);
    SharedMemory get_cpu_memory();

    void tick();
};


#endif //MIPS86CORE_MIPS32_CPU_H
