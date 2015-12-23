#ifndef MIPS86CORE_ARITHMETIC_IMM_H
#define MIPS86CORE_ARITHMETIC_IMM_H

case OP_ADDI:
    write_reg(params.rt, read_reg_signed(params.rs) + params.sign_ext_imm);
    break;

case OP_ADDIU:
    write_reg(params.rt, read_reg_unsigned(params.rs) + params.sign_ext_imm);
    break;

case OP_LUI:
    write_reg(params.rt, params.signed_imm << 16);
    break;

case OP_ANDI:
    write_reg(params.rt, read_reg_unsigned(params.rs) & params.immediate);
    break;

case OP_ORI:
    write_reg(params.rt, read_reg_unsigned(params.rs) | params.immediate);
    break;

case OP_XORI:
    write_reg(params.rt, read_reg_unsigned(params.rs) ^ params.immediate);
    break;

case OP_SLTI:
    write_reg(params.rt, read_reg_signed(params.rs) < params.signed_sign_ext_imm);
    break;

case OP_SLTIU:
    write_reg(params.rt, read_reg_unsigned(params.rs) < params.sign_ext_imm);
    break;

#endif //MIPS86CORE_ARITHMETIC_IMM_H
