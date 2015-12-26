#include "Instruction.h"

Instruction::Instruction(uint32_t instruction, uint32_t pc) : instruction(instruction) {
    this->opcode = static_cast<uint8_t>((instruction >> 26) & 0b111111);
    this->rs = static_cast<uint8_t>((instruction >> 21) & 0b11111);
    this->rt = static_cast<uint8_t>((instruction >> 16) & 0b11111);
    this->rd = static_cast<uint8_t>((instruction >> 11) & 0b11111);
    this->shamt = static_cast<uint8_t>((instruction >> 6) & 0b11111);
    this->funct = static_cast<uint8_t>(instruction & 0b111111);
    this->immediate = static_cast<uint16_t>(instruction & 0xffff);
    this->signed_imm = static_cast<int16_t>(this->immediate);
    this->sign_ext_imm = static_cast<uint32_t>((0xFFFF0000 & ~((this->immediate >> 15 != 0) - 1)) + this->immediate);
    this->signed_sign_ext_imm = static_cast<int32_t>(this->sign_ext_imm);
    this->target = (pc & 0xf0000000) | ((instruction & 0x3ffffff) << 2);
}