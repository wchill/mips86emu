#ifndef MIPS86CORE_SHIFT_H
#define MIPS86CORE_SHIFT_H

case OP0_SLLV:
    params.shamt = static_cast<uint8_t>(read_reg_unsigned(params.rs) & 0b11111);

case OP0_SLL:
    write_reg(params.rd, read_reg_unsigned(params.rt) << params.shamt);
    break;

case OP0_SRLV:
    params.shamt = static_cast<uint8_t>(read_reg_unsigned(params.rs) & 0b11111);

case OP0_SRL:
    write_reg(params.rd, read_reg_unsigned(params.rt) >> params.shamt);
    break;

case OP0_SRAV:
    params.shamt = static_cast<uint8_t>(read_reg_unsigned(params.rs) & 0b11111);

case OP0_SRA:
    write_reg(params.rd, (read_reg_signed(params.rt) >> params.shamt));
    break;

#endif //MIPS86CORE_SHIFT_H
