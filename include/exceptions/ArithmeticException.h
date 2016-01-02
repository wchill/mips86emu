#ifndef MIPS86CORE_ARITHMETICEXCEPTION_H
#define MIPS86CORE_ARITHMETICEXCEPTION_H

#include "CpuException.h"

class ArithmeticException : public CpuException {
public:
    ArithmeticException(uint32_t instruction, uint32_t pc) : CpuException(instruction, pc) {}

    virtual const char* what() const throw() {
        // const variables don't work with cppformat
        uint32_t inst = instruction;
        uint32_t addr = pc;
        return fmt::sprintf("An arithmetic exception occurred @ %#08x (instruction: %#08x)", inst, addr).c_str();
    }
};

#endif //MIPS86CORE_ARITHMETICEXCEPTION_H
