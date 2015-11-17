#ifndef MIPS_INSTRUCTIONS
#define MIPS_INSTRUCTIONS

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

void instr_shift_left_logical();
void instr_shift_left_logical_variable();
void instr_shift_right_logical();
void instr_shift_right_logical_variable();
void instr_shift_right_arith();
void instr_shift_right_arith_variable();
void instr_add();
void instr_add_unsigned();
void instr_sub();
void instr_sub_unsigned();
void instr_multiply();
void instr_multiply_unsigned();
void instr_divide();
void instr_divide_unsigned();
void instr_and();
void instr_or();
void instr_nor();
void instr_xor();
void instr_set_less_than();
void instr_set_less_than_unsigned();
void instr_move_from_hi();
void instr_move_to_hi();
void instr_move_from_lo();
void instr_move_to_lo();
void instr_jump_reg();
void instr_jump_and_link_reg();
void instr_syscall();
void instr_break();
void instr_unimplemented();

int execute_syscall(uint32_t service);

#endif
