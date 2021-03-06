#ifndef MIPS86CORE_MIPS32_CPU_H
#define MIPS86CORE_MIPS32_CPU_H

#include <cstdint>
#include <iostream>
#include <vector>
#include "Instruction.h"
#include "MIPS32_Coprocessor.h"
#include "MIPS32_Coprocessor0.h"
#include "MIPS32_Coprocessor1.h"
#include "exceptions/CpuException.h"
#include "exceptions/ArithmeticException.h"
#include "MemoryMappedDevice.h"
#include "Memory.h"
#include "mips.h"

using std::cout;
using std::cerr;
using std::cin;
using std::endl;

// Registers
#define NUM_REGISTERS 32

#define R_ZERO  0   // Zero register
#define R_AT    1   // Assembler temporary register
#define R_V0    2   // Function results/expression evaluation registers $v0-$v1
#define R_V1    3
#define R_A0    4   // Argument registers $a0-$a3
#define R_A1    5
#define R_A2    6
#define R_A3    7
#define R_T0    8   // Temporary registers $t0-$t7
#define R_T1    9
#define R_T2    10
#define R_T3    11
#define R_T4    12
#define R_T5    13
#define R_T6    14
#define R_T7    15
#define R_S0    16  // Saved temporary registers $s0-$s7
#define R_S1    17
#define R_S2    18
#define R_S3    19
#define R_S4    20
#define R_S5    21
#define R_S6    22
#define R_S7    23
#define R_T8    24  // Temporaries $t8-$t9
#define R_T9    25
#define R_K0    26  // Kernel registers $k0-$k1
#define R_K1    27
#define R_GP    28  // Global pointer
#define R_SP    29  // Stack pointer
#define R_FP    30  // Frame pointer
#define R_RA    31  // Return address

class MIPS32_Cpu {
private:
    bool little_endian = false;
    bool load_linked = false;
    uint32_t last_pc;
    uint32_t pc;
    uint32_t registers[NUM_REGISTERS];
    uint64_t acc;
    SharedMemory memory;
    MIPS32_Coprocessor *coprocessors[4];

    // temporary storage for functions
    uint8_t bytes[4];
    uint16_t shorts[4];
    uint32_t words[4];
    uint64_t dwords[4];
    int8_t bytes_s[4];
    int16_t shorts_s[4];
    int32_t words_s[4];
    int64_t dwords_s[4];

    std::vector<std::shared_ptr<MemoryMappedDevice>> memory_mapped_devices;

    // Bitwise ANDing with ~((reg != 0) - 1) means all writes to $0 will be 0
    inline void write_reg(int reg, uint32_t word) { registers[reg] = word & ~((reg != 0) - 1); }
    inline void write_reg(int reg, int32_t word) { registers[reg] = static_cast<uint32_t>(word) & ~((reg != 0) - 1); }
    inline void write_acc(uint64_t dword) { acc = dword; }
    inline void write_acc(int64_t dword) { acc = static_cast<uint64_t>(dword); }
    inline void write_hi(uint32_t word) { acc = (static_cast<uint64_t>(word) << 32) | (acc & 0xFFFFFFFF); }
    inline void write_hi(int32_t word) { acc = (static_cast<uint64_t>(word) << 32) | (acc & 0xFFFFFFFF); }
    inline void write_lo(uint32_t word) { acc = (acc & 0xFFFFFFFF00000000) | word; }
    inline void write_lo(int32_t word) { acc = (acc & 0xFFFFFFFF00000000) | static_cast<uint32_t>(word); }
    inline int32_t read_reg_signed(int reg) { return static_cast<int32_t>(registers[reg]); }
    inline uint32_t read_reg_unsigned(int reg) { return registers[reg]; }
    inline int64_t read_acc_signed() { return static_cast<int64_t>(acc); }
    inline uint64_t read_acc_unsigned() { return acc; }
    inline int32_t read_hi_signed() { return static_cast<int32_t>(acc >> 32); }
    inline uint32_t read_hi_unsigned() { return static_cast<uint32_t>(acc >> 32); }
    inline int32_t read_lo_signed() { return static_cast<int32_t>(acc & 0xFFFFFFFF); }
    inline uint32_t read_lo_unsigned() { return static_cast<uint32_t>(acc & 0xFFFFFFFF); }

    // TODO: Set proper endianness at compile time
    inline uint32_t get_next_instruction() { return memory->read_word(pc += 4); }

    void branch(uint16_t immediate) {
        uint32_t branch_addr = ((uint32_t) immediate) << 2;
        branch_addr |= (uint32_t) (0xFFFC0000 * (immediate >> 15));
        uint32_t new_pc = (uint32_t) ((int32_t) pc + (int32_t) branch_addr);
        //cout << "    - Branching: executing branch delay slot" << endl;
        // TODO: handle return code from tick
        tick();
        pc = new_pc;
        //cout << fmt::sprintf("    - Branching: jumping to %#08x", pc + 4) << endl;
    }

public:
    MIPS32_Cpu(bool little_endian = false);
    virtual ~MIPS32_Cpu();

    void reset();
    void add_memory_mapped_device(std::shared_ptr<MemoryMappedDevice> device);
    SharedMemory get_cpu_memory();

    bool tick();
    void execute(uint32_t instruction);
};


#endif //MIPS86CORE_MIPS32_CPU_H
