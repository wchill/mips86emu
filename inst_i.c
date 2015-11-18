#include <stdio.h>
#include "memory.h"
#include "register.h"
#include "inst_i.h"

void parse_i_inst(uint32_t inst, i_inst *params) {
    params->instruction = inst;
    params->opcode = (uint8_t) ((inst >> 26) & 0b11111);
    params->rs = (uint8_t) ((inst >> 21) & 0b1111);    
    params->rt = (uint8_t) ((inst >> 16) & 0b1111);    
    params->immediate = (uint16_t) (inst & 0xffff);
    params->sign_ext_imm = (uint32_t) (0xFFFF0000 * (params->immediate >> 15) + params->immediate);
}

void instr_add_immediate_unsigned(i_inst params) {
    write_reg(params.rt, read_reg_unsigned(params.rs) + params.sign_ext_imm);
}

void instr_load_upper_immediate(i_inst params) {
    write_reg(params.rt, ((uint32_t) params.immediate) << 16);
}

void instr_or_immediate(i_inst params) {
    write_reg(params.rt, read_reg_unsigned(params.rs) | params.immediate);
}

void instr_load_word(i_inst params) {
    write_reg(params.rt, read_word(read_reg_signed(params.rs) + (int16_t) params.immediate));
}

void instr_store_word(i_inst params) {
    write_word(read_reg_signed(params.rs) + (int16_t) params.immediate, read_reg_unsigned(params.rt));
}

void instr_unimplemented_i(i_inst params) {
    printf("Opcode 0x%02x unimplemented\n", params.opcode);
    printf("Instruction: 0x%08x\n", params.instruction);
}

void (*i_inst_table[])(i_inst) = {
    instr_unimplemented_i,          // 0x00 - OP_OTHER0 (r-type)
    instr_unimplemented_i,          // 0x01 - OP_OTHER1
    instr_unimplemented_i,          // 0x02 - OP_J (j-type)
    instr_unimplemented_i,          // 0x03 - OP_JAL (j-type)
    instr_unimplemented_i,          // 0x04 - OP_BEQ
    instr_unimplemented_i,          // 0x05 - OP_BNE
    instr_unimplemented_i,          // 0x06 - OP_BLEZ
    instr_unimplemented_i,          // 0x07 - OP_BGTZ
    instr_unimplemented_i,          // 0x08 - OP_ADDI
    instr_add_immediate_unsigned,   // 0x09 - OP_ADDIU
    instr_unimplemented_i,          // 0x0a - OP_SLTI
    instr_unimplemented_i,          // 0x0b - OP_SLTIU
    instr_unimplemented_i,          // 0x0c - OP_ANDI
    instr_or_immediate,             // 0x0d - OP_ORI
    instr_unimplemented_i,          // 0x0e - OP_XORI
    instr_load_upper_immediate,     // 0x0f - OP_LUI
    instr_unimplemented_i,          // 0x10 - OP_Z0
    instr_unimplemented_i,          // 0x11 - OP_Z1
    instr_unimplemented_i,          // 0x12 - OP_Z2
    instr_unimplemented_i,          // 0x13 - OP_Z3
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_unimplemented_i,          // 0x20 - OP_LB
    instr_unimplemented_i,          // 0x21 - OP_LH
    instr_unimplemented_i,          // 0x22 - OP_LWL
    instr_load_word,                // 0x23 - OP_LW
    instr_unimplemented_i,          // 0x24 - OP_LBU
    instr_unimplemented_i,          // 0x25 - OP_LHU
    instr_unimplemented_i,          // 0x26 - OP_LWR
    instr_unimplemented_i,
    instr_unimplemented_i,          // 0x28 - OP_SB
    instr_unimplemented_i,          // 0x29 - OP_SH
    instr_unimplemented_i,          // 0x2a - OP_SWL
    instr_store_word,               // 0x2b - OP_SW
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_unimplemented_i,          // 0x2e - OP_SWR
    instr_unimplemented_i,
    instr_unimplemented_i,          // 0x30 - OP_LWC0
    instr_unimplemented_i,          // 0x31 - OP_LWC1
    instr_unimplemented_i,          // 0x32 - OP_LWC2
    instr_unimplemented_i,          // 0x33 - OP_LWC3
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_unimplemented_i,          // 0x38 - OP_SWC0
    instr_unimplemented_i,          // 0x39 - OP_SWC1
    instr_unimplemented_i,          // 0x3a - OP_SWC2
    instr_unimplemented_i           // 0x3b - OP_SWC3
};
