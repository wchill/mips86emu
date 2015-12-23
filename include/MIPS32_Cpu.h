#ifndef MIPS86CORE_MIPS32_CPU_H
#define MIPS86CORE_MIPS32_CPU_H

#include <cstdint>
#include "memory.h"

class MIPS32_Cpu {
private:
    uint32_t pc;
    uint32_t registers[32];
    Memory memory;
};


#endif //MIPS86CORE_MIPS32_CPU_H
