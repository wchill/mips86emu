#ifndef MIPS86CORE_MIPS32_COPROCESSOR0_H
#define MIPS86CORE_MIPS32_COPROCESSOR0_H

#include "MIPS32_Coprocessor.h"

#define CP0_INDEX 0
#define CP0_RANDOM 1
#define CP0_ENTRYLO0 2
#define CP0_ENTRYLO1 3
#define CP0_CONTEXT 4
#define CP0_PAGEMASK 5
#define CP0_WIRED 6

#define CP0_BADVADDR 8
#define CP0_COUNT 9
#define CP0_ENTRYHI 10
#define CP0_COMPARE 11
#define CP0_STATUS 12
#define CP0_CAUSE 13
#define CP0_EPC 14
#define CP0_PRID 15
#define CP0_CONFIG 16
#define CP0_LLADDR 17
#define CP0_WATCHLO 18
#define CP0_WATCHHI 19

#define CP0_DEBUG 23
#define CP0_DEPC 24
#define CP0_PERFCNT 25
#define CP0_ERRCTL 26
#define CP0_CACHEERR 27
#define CP0_TAGLO 28
#define CP0_TAGHI 29
#define CP0_ERROREPC 30
#define CP0_DESAVE 31

class MIPS32_Coprocessor0 : public MIPS32_Coprocessor {
protected:
    virtual void write_reg(uint8_t reg, uint32_t word);
    virtual uint32_t read_reg(uint8_t reg);
    virtual void write_control(uint8_t reg, uint32_t word);
    virtual uint32_t read_control(uint8_t reg);
public:
    MIPS32_Coprocessor0(SharedMemory memory);

    virtual void load_word(Instruction params, uint32_t addr);
    virtual void store_word(Instruction params, uint32_t addr);
};

#endif //MIPS86CORE_MIPS32_COPROCESSOR0_H
