#ifndef MIPS86CORE_LOADSTORE_H
#define MIPS86CORE_LOADSTORE_H

case OP_LW:
    write_reg(params.rt, memory->read_word(read_reg_signed(params.rs) + params.signed_imm));
    break;
case OP_LB:
    bytes[0] = memory->read_byte(read_reg_signed(params.rs) + params.signed_imm);
    write_reg(params.rt, static_cast<uint32_t>(0xFFFFFF00 * (bytes[0] >> 7) + bytes[0]));
    break;
case OP_SW:
    memory->write_word(read_reg_signed(params.rs) + params.signed_imm, read_reg_unsigned(params.rt));
    break;
case OP_SB:
    memory->write_byte(read_reg_signed(params.rs) + params.signed_imm, static_cast<uint8_t>(read_reg_unsigned(params.rt) & 0xFF));
    break;
/*
case OP_LWC1:
case OP_LWC2:
case OP_SWC1:
case OP_SWC2:
*/

#endif //MIPS86CORE_LOADSTORE_H
