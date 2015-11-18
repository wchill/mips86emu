#ifndef MIPS_I_INST
#define MIPS_I_INST

typedef struct {
    uint32_t instruction;
    uint8_t opcode;
    uint8_t rs;
    uint8_t rt;
    uint16_t immediate;
    uint32_t sign_ext_imm;
} i_inst;

extern void (*i_inst_table[])(i_inst);

void parse_i_inst(uint32_t inst, i_inst *params);

void instr_add_immediate_unsigned(i_inst params);
void instr_load_upper_immediate(i_inst params);
void instr_or_immediate(i_inst params);
void instr_load_word(i_inst params);
void instr_store_word(i_inst params);
void instr_unimplemented_i(i_inst params);

#endif
