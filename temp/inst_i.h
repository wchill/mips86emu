#ifndef MIPS_I_INST
#define MIPS_I_INST

#include <stdint.h>

extern void (*i_inst_table[])(i_inst);
extern void (*branch_inst_table[])(i_inst);

void parse_i_inst(uint32_t inst, i_inst *params);
void instr_branch_other(i_inst params);

void instr_branch_equal(i_inst params);
void instr_branch_not_equal(i_inst params);
void instr_branch_less_than_zero(i_inst params);
void instr_branch_greater_than_equal_zero(i_inst params);
void instr_branch_less_than_zero_and_link(i_inst params);
void instr_branch_greater_than_equal_zero_and_link(i_inst params);
void instr_add_immediate(i_inst params);
void instr_add_immediate_unsigned(i_inst params);
void instr_load_upper_immediate(i_inst params);
void instr_and_immediate(i_inst params);
void instr_or_immediate(i_inst params);
void instr_xor_immediate(i_inst params);
void instr_load_word(i_inst params);
void instr_store_word(i_inst params);
void instr_load_byte(i_inst params);
void instr_store_byte(i_inst params);
void instr_set_less_than_immutable(i_inst params);
void instr_set_less_than_immutable_unsigned(i_inst params);
void instr_unimplemented_i(i_inst params);

#endif
