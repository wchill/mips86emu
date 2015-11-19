#ifndef MIPS_COPROCESSOR
#define MIPS_COPROCESSOR

#include <stdlib.h>
#include "mips.h"
#include "instructions.h"

typedef struct {
    uint32_t *registers;
    uint32_t *control;
    uint8_t num_registers;
    uint8_t num_control;
} mips_coprocessor;

mips_coprocessor *coprocessors[4];

extern void (*cop_inst_table[])(uint8_t, r_inst);

void instr_load_word_coprocessor(i_inst params);
void instr_store_word_coprocessor(i_inst params);
/* Not valid in MIPS-I
void load_double_coprocessor(i_inst params);
void store_double_coprocessor(i_inst params);
*/

void write_reg_coprocessor(uint8_t cop_num, uint8_t reg, uint32_t word);
uint32_t read_reg_coprocessor(uint8_t cop_num, uint8_t reg);
void write_control_coprocessor(uint8_t cop_num, uint8_t reg, uint32_t word);
uint32_t read_control_coprocessor(uint8_t cop_num, uint8_t reg);

void instr_cop_z(i_inst params);

void instr_move_to_coprocessor(uint8_t cop_num, r_inst params);
void instr_move_from_coprocessor(uint8_t cop_num, r_inst params);
/* Not valid in MIPS-I
void move_to_coprocessor_double(i_inst params);
void move_from_coprocessor_double(i_inst params);
*/

void instr_unimplemented_coprocessor(uint8_t cop_num, r_inst params);

#endif
