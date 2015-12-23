#include <stdio.h>
#include "memory.h"
#include "register.h"
#include "inst_i.h"
#include "coprocessor.h"

void parse_i_inst(uint32_t inst, i_inst *params) {
    params->instruction = inst;
    params->opcode = (uint8_t) ((inst >> 26) & 0b111111);
    params->rs = (uint8_t) ((inst >> 21) & 0b11111);    
    params->rt = (uint8_t) ((inst >> 16) & 0b11111);    
    params->immediate = (uint16_t) (inst & 0xffff);
    params->sign_ext_imm = (uint32_t) (0xFFFF0000 * (params->immediate >> 15) + params->immediate);
}

void branch(uint16_t immediate) {
    uint32_t branch_addr = ((uint32_t) immediate) << 2;
    branch_addr |= (uint32_t) (0xFFFC0000 * (immediate >> 15));
    pc = (uint32_t) ((int32_t) pc + (int32_t) branch_addr) + 4;
    branch_taken = true;
}

void instr_branch_equal(i_inst params) {
    if(read_reg_unsigned(params.rs) == read_reg_unsigned(params.rt)) {
        branch(params.immediate);
    }
}

void instr_branch_not_equal(i_inst params) {
    if(read_reg_unsigned(params.rs) != read_reg_unsigned(params.rt)) {
        branch(params.immediate);
    }
}

void instr_branch_less_than_equal_zero(i_inst params) {
    if(read_reg_signed(params.rs) <= 0) {
        branch(params.immediate);
    }
}

void instr_branch_less_than_zero(i_inst params) {
    if(read_reg_signed(params.rs) < 0) {
        branch(params.immediate);
    }
}

void instr_branch_greater_than_equal_zero(i_inst params) {
    if(read_reg_signed(params.rs) >= 0) {
        branch(params.immediate);
    }
}

void instr_branch_greater_than_zero(i_inst params) {
    if(read_reg_signed(params.rs) > 0) {
        branch(params.immediate);
    }
}

void instr_branch_less_than_zero_and_link(i_inst params) {
    uint32_t old_pc = pc;
    instr_branch_less_than_zero(params);
    if(old_pc != pc) {
        write_reg(R_RA, old_pc + 8);
    }
}

void instr_branch_greater_than_equal_zero_and_link(i_inst params) {
    uint32_t old_pc = pc;
    instr_branch_greater_than_equal_zero(params);
    if(old_pc != pc) {
        write_reg(R_RA, old_pc + 8);
    }
}

void instr_branch_other(i_inst params) {
    (*branch_inst_table[params.rt])(params);
}

// TODO: overflow exception
void instr_add_immediate(i_inst params) {
    write_reg(params.rt, read_reg_signed(params.rs) + params.sign_ext_imm);
}

void instr_add_immediate_unsigned(i_inst params) {
    write_reg(params.rt, read_reg_unsigned(params.rs) + params.sign_ext_imm);
}

void instr_load_upper_immediate(i_inst params) {
    write_reg(params.rt, ((uint32_t) params.immediate) << 16);
}

void instr_and_immediate(i_inst params) {
    write_reg(params.rt, read_reg_unsigned(params.rs) & params.immediate);
}

void instr_or_immediate(i_inst params) {
    write_reg(params.rt, read_reg_unsigned(params.rs) | params.immediate);
}

void instr_xor_immediate(i_inst params) {
    write_reg(params.rt, read_reg_unsigned(params.rs) ^ params.immediate);
}

void instr_load_word(i_inst params) {
    write_reg(params.rt, read_word(read_reg_signed(params.rs) + (int16_t) params.immediate));
}

void instr_store_word(i_inst params) {
    write_word(read_reg_signed(params.rs) + (int16_t) params.immediate, read_reg_unsigned(params.rt));
}

void instr_load_byte(i_inst params) {
    uint8_t b = (uint8_t) read_byte(read_reg_signed(params.rs) + (int16_t) params.immediate);
    write_reg(params.rt, (uint32_t) (0xFFFFFF00 * (b >> 7) + b));
}

void instr_store_byte(i_inst params) {
    int32_t addr = read_reg_signed(params.rs) + (int16_t) params.immediate;
    write_byte(addr, (uint8_t) (read_reg_unsigned(params.rt) & 0xFF));
}

void instr_set_less_than_immutable(i_inst params) {
    write_reg(params.rt, (read_reg_signed(params.rs) < (int32_t) params.sign_ext_imm));
}

void instr_set_less_than_immutable_unsigned(i_inst params) {
    write_reg(params.rt, read_reg_unsigned(params.rs) < params.sign_ext_imm);
}

void instr_unimplemented_i(i_inst params) {
    printf("Unimplemented instruction 0x%08x @ 0x%08x\n", params.instruction, pc);
    printf("Opcode: 0x%02x\nrs: 0x%02x\nrt: 0x%02x\nimm: 0x%02x\n", params.opcode, params.rs, params.rt, params.immediate);
}

void (*i_inst_table[])(i_inst) = {
    instr_unimplemented_i,          // 0x00 - OP_OTHER0 (r-type)
    instr_branch_other,             // 0x01 - OP_OTHER1 (branch)
    instr_unimplemented_i,          // 0x02 - OP_J (j-type)
    instr_unimplemented_i,          // 0x03 - OP_JAL (j-type)
    instr_branch_equal,             // 0x04 - OP_BEQ
    instr_branch_not_equal,         // 0x05 - OP_BNE
    instr_branch_less_than_equal_zero,  // 0x06 - OP_BLEZ
    instr_branch_greater_than_zero, // 0x07 - OP_BGTZ
    instr_add_immediate,            // 0x08 - OP_ADDI
    instr_add_immediate_unsigned,   // 0x09 - OP_ADDIU
    instr_set_less_than_immutable,  // 0x0a - OP_SLTI
    instr_set_less_than_immutable_unsigned, // 0x0b - OP_SLTIU
    instr_and_immediate,            // 0x0c - OP_ANDI
    instr_or_immediate,             // 0x0d - OP_ORI
    instr_xor_immediate,            // 0x0e - OP_XORI
    instr_load_upper_immediate,     // 0x0f - OP_LUI
    instr_cop_z,                    // 0x10 - OP_Z0
    instr_cop_z,                    // 0x11 - OP_Z1
    instr_cop_z,                    // 0x12 - OP_Z2
    instr_cop_z,                    // 0x13 - OP_Z3
    instr_unimplemented_i,          // 0x14
    instr_unimplemented_i,          // 0x15
    instr_unimplemented_i,          // 0x16
    instr_unimplemented_i,          // 0x17
    instr_unimplemented_i,          // 0x18
    instr_unimplemented_i,          // 0x19
    instr_unimplemented_i,          // 0x1a
    instr_unimplemented_i,          // 0x1b
    instr_unimplemented_i,          // 0x1c - OP_SPECIAL2 (r-type)
    instr_unimplemented_i,          // 0x1d - OP_JALX
    instr_unimplemented_i,
    instr_unimplemented_i,          // 0x1f - OP_SPECIAL3
    instr_load_byte,                // 0x20 - OP_LB
    instr_unimplemented_i,          // 0x21 - OP_LH
    instr_unimplemented_i,          // 0x22 - OP_LWL
    instr_load_word,                // 0x23 - OP_LW
    instr_unimplemented_i,          // 0x24 - OP_LBU
    instr_unimplemented_i,          // 0x25 - OP_LHU
    instr_unimplemented_i,          // 0x26 - OP_LWR
    instr_unimplemented_i,
    instr_store_byte,               // 0x28 - OP_SB
    instr_unimplemented_i,          // 0x29 - OP_SH
    instr_unimplemented_i,          // 0x2a - OP_SWL
    instr_store_word,               // 0x2b - OP_SW
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_unimplemented_i,          // 0x2e - OP_SWR
    instr_unimplemented_i,          // 0x2f - OP_CACHE
    instr_unimplemented_i,          // 0x30 - OP_LL
    instr_load_word_coprocessor,    // 0x31 - OP_LWC1
    instr_load_word_coprocessor,    // 0x32 - OP_LWC2
    instr_unimplemented_i,          // 0x33 - OP_PREF
    instr_unimplemented_i,
    instr_unimplemented_i,          // 0x35 - OP_LDC1
    instr_unimplemented_i,          // 0x36 - OP_LDC2
    instr_unimplemented_i,
    instr_unimplemented_i,          // 0x38 - OP_SC
    instr_store_word_coprocessor,   // 0x39 - OP_SWC1
    instr_store_word_coprocessor,   // 0x3a - OP_SWC2
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_unimplemented_i,          // 0x3d - OP_SDC1
    instr_unimplemented_i           // 0x3e - OP_SDC2
};

void (*branch_inst_table[])(i_inst) = {
    instr_branch_less_than_zero,                    // 0x00 - OP1_BLTZ
    instr_branch_greater_than_equal_zero,           // 0x01 - OP1_BGEZ
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
    instr_unimplemented_i,
    instr_unimplemented_i,
    instr_branch_less_than_zero_and_link,           // 0x10 - OP1_BLTZAL
    instr_branch_greater_than_equal_zero_and_link,  // 0x11 - OP1_BGEZAL
};
