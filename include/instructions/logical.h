#ifndef MIPS86CORE_LOGICAL_H
#define MIPS86CORE_LOGICAL_H

case OP0_AND:
    write_reg(params.rd, read_reg_unsigned(params.rs) & read_reg_unsigned(params.rt));
    break;

case OP0_OR:
    write_reg(params.rd, read_reg_unsigned(params.rs) | read_reg_unsigned(params.rt));
    break;

case OP0_XOR:
    write_reg(params.rd, read_reg_unsigned(params.rs) ^ read_reg_unsigned(params.rt));
    break;

case OP0_NOR:
    write_reg(params.rd, ~(read_reg_unsigned(params.rs) | read_reg_unsigned(params.rt)));
    break;

case OP0_SLT:
    write_reg(params.rd, read_reg_signed(params.rs) < read_reg_signed(params.rt));
    break;

case OP0_SLTU:
    write_reg(params.rd, read_reg_unsigned(params.rs) < read_reg_unsigned(params.rt));
    break;

#endif //MIPS86CORE_LOGICAL_H
