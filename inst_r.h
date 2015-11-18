#ifndef MIPS_R_INST
#define MIPS_R_INST

#include <stdint.h>
#include "mips.h"

typedef struct {
    uint32_t instruction;
    uint8_t rs;
    uint8_t rt;
    uint8_t rd;
    uint8_t shamt;
    uint8_t funct;
} r_inst;

extern void (*r_inst_table[])(r_inst);

void parse_r_inst(uint32_t inst, r_inst *params);

void instr_shift_left_logical(r_inst params);
void instr_shift_left_logical_variable(r_inst params);
void instr_shift_right_logical(r_inst params);
void instr_shift_right_logical_variable(r_inst params);
void instr_shift_right_arith(r_inst params);
void instr_shift_right_arith_variable(r_inst params);
void instr_add(r_inst params);
void instr_add_unsigned(r_inst params);
void instr_sub(r_inst params);
void instr_sub_unsigned(r_inst params);
void instr_multiply(r_inst params);
void instr_multiply_unsigned(r_inst params);
void instr_divide(r_inst params);
void instr_divide_unsigned(r_inst params);
void instr_and(r_inst params);
void instr_or(r_inst params);
void instr_nor(r_inst params);
void instr_xor(r_inst params);
void instr_set_less_than(r_inst params);
void instr_set_less_than_unsigned(r_inst params);
void instr_move_from_hi(r_inst params);
void instr_move_to_hi(r_inst params);
void instr_move_from_lo(r_inst params);
void instr_move_to_lo(r_inst params);
void instr_jump_reg(r_inst params);
void instr_jump_and_link_reg(r_inst params);
void instr_syscall(r_inst params);
void instr_break(r_inst params);
void instr_unimplemented_r(r_inst params);

int execute_syscall(uint32_t service);

#endif
