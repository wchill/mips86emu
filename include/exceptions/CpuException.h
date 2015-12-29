#ifndef MIPS86CORE_CPUEXCEPTION_H
#define MIPS86CORE_CPUEXCEPTION_H

#include <cstdint>

class CpuException : public std::exception {
public:
    const uint32_t instruction;
    const uint32_t pc;

    CpuException(uint32_t instruction, uint32_t pc) : instruction(instruction), pc(pc) {}

    virtual const char* what() const throw() {
        // const variables don't work with cppformat
        uint32_t inst = instruction;
        uint32_t addr = pc;
        return fmt::sprintf("A CPU exception occurred @ %#08x (instruction: %#08x)", inst, addr).c_str();
    }
};

#endif //MIPS86CORE_CPUEXCEPTION_H
