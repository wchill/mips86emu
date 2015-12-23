#include <stdio.h>
#include "coprocessor.h"
#include "memory_old.h"
#include "register.h"

void write_reg_coprocessor(uint8_t cop_num, uint8_t reg, uint32_t word) {
    if(coprocessors[cop_num] && coprocessors[cop_num]->num_registers >= reg) {
        coprocessors[cop_num]->registers[reg] = word;
    } else {
        printf("Attempted to access invalid register %d on coprocessor %d\n", reg, cop_num);
        exit(1);
    }
}

uint32_t read_reg_coprocessor(uint8_t cop_num, uint8_t reg) {
    if(coprocessors[cop_num] && coprocessors[cop_num]->num_registers >= reg) {
        return coprocessors[cop_num]->registers[reg];
    } else {
        printf("Attempted to access invalid register %d on coprocessor %d\n", reg, cop_num);
        exit(1);
    }
}

void write_control_coprocessor(uint8_t cop_num, uint8_t reg, uint32_t word) {
    if(coprocessors[cop_num] && coprocessors[cop_num]->num_control >= reg) {
        coprocessors[cop_num]->control[reg] = word;
    } else {
        printf("Attempted to access invalid control register %d on coprocessor %d\n", reg, cop_num);
        exit(1);
    }
}

uint32_t read_control_coprocessor(uint8_t cop_num, uint8_t reg) {
    if(coprocessors[cop_num] && coprocessors[cop_num]->num_control >= reg) {
        return coprocessors[cop_num]->control[reg];
    } else {
        printf("Attempted to access invalid control register %d on coprocessor %d\n", reg, cop_num);
        exit(1);
    }
}


void instr_load_word_coprocessor(i_inst params) {
    int cop_num = params.opcode - 0x30;
    write_reg_coprocessor(cop_num, params.rt, read_word(read_reg_unsigned(params.rs) + params.sign_ext_imm));
}

void instr_store_word_coprocessor(i_inst params) {
    int cop_num = params.opcode - 0x38;
    write_word(read_reg_unsigned(params.rs) + params.sign_ext_imm, read_reg_coprocessor(cop_num, params.rt));
}

void instr_cop_z(i_inst params) {
    r_inst r_params;
    parse_r_inst(params.instruction, &r_params);
    (*cop_inst_table[params.rs])(params.opcode - 0x10, r_params);
}

void instr_move_from_coprocessor(uint8_t cop_num, r_inst params) {
    write_reg(params.rt, read_reg_coprocessor(cop_num, params.rd));
}

void instr_control_from_coprocessor(uint8_t cop_num, r_inst params) {
    write_reg(params.rt, read_control_coprocessor(cop_num, params.rd));
}

void instr_move_to_coprocessor(uint8_t cop_num, r_inst params) {
    write_reg_coprocessor(cop_num, params.rd, read_reg_unsigned(params.rt));
}

void instr_control_to_coprocessor(uint8_t cop_num, r_inst params) {
    write_control_coprocessor(cop_num, params.rd, read_reg_unsigned(params.rt));
}

void instr_unimplemented_coprocessor(uint8_t cop_num, r_inst params) {
    printf("Coprocessor %d instruction 0x%02x unimplemented\n", cop_num, params.rs);
    printf("Instruction: 0x%08x\n", params.instruction);
}

void (*cop_inst_table[])(uint8_t, r_inst) = {
    instr_move_from_coprocessor,            // 0x00 - OPZ_MFCZ
    instr_unimplemented_coprocessor,
    instr_control_from_coprocessor,         // 0x02 - OPZ_CFCZ
    instr_unimplemented_coprocessor,
    instr_move_to_coprocessor,              // 0x04 - OPZ_MTCZ
    instr_unimplemented_coprocessor,
    instr_control_to_coprocessor,           // 0x06 - OPZ_CTCZ
    instr_unimplemented_coprocessor,
    instr_unimplemented_coprocessor,        // 0x08 - OPZ_BCZ
    instr_unimplemented_coprocessor,
    instr_unimplemented_coprocessor,
    instr_unimplemented_coprocessor,
    instr_unimplemented_coprocessor,
    instr_unimplemented_coprocessor,
    instr_unimplemented_coprocessor,
    instr_unimplemented_coprocessor,
    instr_unimplemented_coprocessor,        // 0x10 - OPZ_COPZS
    instr_unimplemented_coprocessor         // 0x11 - OPZ_COPZD
};
