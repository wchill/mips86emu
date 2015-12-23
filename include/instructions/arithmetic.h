#ifndef MIPS86CORE_ARITHMETIC_H
#define MIPS86CORE_ARITHMETIC_H

case OP0_ADD:
    write_reg(params.rd, read_reg_signed(params.rs) + read_reg_signed(params.rt));
    break;

case OP0_ADDU:
    write_reg(params.rd, read_reg_unsigned(params.rs) + read_reg_unsigned(params.rt));
    break;

case OP0_SUB:
    write_reg(params.rd, read_reg_signed(params.rs) - read_reg_signed(params.rt));
    break;

case OP0_SUBU:
    write_reg(params.rd, read_reg_unsigned(params.rs) - read_reg_unsigned(params.rt));
    break;

case OP0_MULT:
    write_acc(static_cast<int64_t>(read_reg_signed(params.rs)) * read_reg_signed(params.rt));
    break;

case OP0_MULTU:
    write_acc(static_cast<uint64_t>(read_reg_unsigned(params.rs)) * read_reg_unsigned(params.rt));
    break;

case OP0_DIV:
    write_lo(read_reg_signed(params.rs) / read_reg_signed(params.rt));
    write_hi(read_reg_signed(params.rs) % read_reg_signed(params.rt));
    break;

case OP0_DIVU:
    write_lo(read_reg_unsigned(params.rs) / read_reg_unsigned(params.rt));
    write_hi(read_reg_unsigned(params.rs) % read_reg_unsigned(params.rt));
    break;

case OP0_MFHI:
    write_reg(params.rd, read_hi_unsigned());
    break;

case OP0_MFLO:
    write_reg(params.rd, read_lo_unsigned());
    break;

case OP0_MTHI:
    write_hi(read_reg_unsigned(params.rs));
    break;

case OP0_MTLO:
    write_lo(read_reg_unsigned(params.rs));
    break;

#endif //MIPS86CORE_ARITHMETIC_H
