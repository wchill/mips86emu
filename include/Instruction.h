#ifndef MIPS86CORE_INSTRUCTION_H
#define MIPS86CORE_INSTRUCTION_H

#include <cstdint>

class Instruction {
public:
    uint32_t instruction;
    uint8_t opcode;
    uint8_t rs;
    uint8_t rt;
    uint8_t rd;
    uint8_t shamt;
    uint8_t funct;
    uint16_t immediate;
    int16_t signed_imm;
    uint32_t sign_ext_imm;
    int32_t signed_sign_ext_imm;
    uint32_t target;

    Instruction(uint32_t instruction, uint32_t pc);
    ~Instruction() {}
};


#endif //MIPS86CORE_INSTRUCTION_H
