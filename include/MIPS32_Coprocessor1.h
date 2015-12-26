#ifndef MIPS86CORE_MIPS32_COPROCESSOR1_H
#define MIPS86CORE_MIPS32_COPROCESSOR1_H

#include "MIPS32_Coprocessor.h"

class MIPS32_Coprocessor1 : public MIPS32_Coprocessor {
protected:
    virtual void write_reg(uint8_t reg, uint32_t word);
    virtual uint32_t read_reg(uint8_t reg);
    virtual void write_control(uint8_t reg, uint32_t word);
    virtual uint32_t read_control(uint8_t reg);
public:
    MIPS32_Coprocessor1(SharedMemory memory);

    virtual void load_word(Instruction params, uint32_t addr);
    virtual void store_word(Instruction params, uint32_t addr);
};

#endif //MIPS86CORE_MIPS32_COPROCESSOR1_H
