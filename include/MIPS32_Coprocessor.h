#ifndef MIPS86CORE_MIPS32_COPROCESSOR_H
#define MIPS86CORE_MIPS32_COPROCESSOR_H

#include "Instruction.h"
#include "Memory.h"

class MIPS32_Coprocessor {
protected:
    uint32_t *registers;
    uint32_t *control_reg;
    uint8_t num_registers;
    uint8_t num_control;
    SharedMemory memory;

    virtual void write_reg(uint8_t reg, uint32_t word) = 0;
    virtual uint32_t read_reg(uint8_t reg) = 0;
    virtual void write_control(uint8_t reg, uint32_t word) = 0;
    virtual uint32_t read_control(uint8_t reg) = 0;
public:
    MIPS32_Coprocessor(SharedMemory memory) : memory(memory), registers(NULL), control_reg(NULL), num_registers(0),
                                              num_control(0) {}

    virtual ~MIPS32_Coprocessor() {
        delete[] registers;
        delete[] control_reg;
        memory.reset();
    }

    virtual void reset() {
        std::fill(registers, registers + num_registers, 0);
        std::fill(control_reg, control_reg + num_control, 0);
    }

    virtual void load_word(Instruction params, uint32_t addr) = 0;
    virtual void store_word(Instruction params, uint32_t addr) = 0;
    inline void move_to_coprocessor(Instruction params, uint32_t word) {
        write_reg(params.rd, word);
    }
    inline uint32_t move_from_coprocessor(Instruction params) {
        return read_reg(params.rd);
    }
    inline void move_to_coprocessor_control(Instruction params, uint32_t word) {
        write_control(params.rd, word);
    }
    inline uint32_t move_from_coprocessor_control(Instruction params) {
        return read_control(params.rd);
    }
};

#endif //MIPS86CORE_MIPS32_COPROCESSOR_H
