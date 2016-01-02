#ifndef MIPS86CORE_LOADSTORE_H
#define MIPS86CORE_LOADSTORE_H
case OP_LL:
    coprocessors[0]->write_reg(OP0_LLADDR, read_reg_signed(params.rs) + params.signed_imm));
    load_linked = true;
case OP_LW:
    write_reg(params.rt, memory->read_word(read_reg_signed(params.rs) + params.signed_imm));
    //cout << fmt::sprintf("Loading word from %#08x", read_reg_signed(params.rs) + params.signed_imm) << endl;
    break;
// TODO: handle endianness
case OP_LH:
    shorts[0] = memory->read_short(read_reg_signed(params.rs) + params.signed_imm);
    write_reg(params.rt, static_cast<uint32_t>(0xFFFF0000 * (shorts[0] >> 15) + shorts[0]));
    break;
case OP_LHU:
    shorts[0] = memory->read_short(read_reg_signed(params.rs) + params.signed_imm);
    write_reg(params.rt, static_cast<uint32_t>(shorts[0]));
    break;
// TODO: handle endianness
case OP_LB:
    bytes[0] = memory->read_byte(read_reg_signed(params.rs) + params.signed_imm);
    write_reg(params.rt, static_cast<uint32_t>(0xFFFFFF00 * (bytes[0] >> 7) + bytes[0]));
    break;
case OP_LBU:
    bytes[0] = memory->read_byte(read_reg_signed(params.rs) + params.signed_imm);
    write_reg(params.rt, static_cast<uint32_t>(bytes[0]));
    break;
case OP_SW:
    memory->write_word(read_reg_signed(params.rs) + params.signed_imm, read_reg_unsigned(params.rt));
    break;
// TODO: handle endianness
case OP_SH:
    memory->write_short(read_reg_signed(params.rs) + params.signed_imm, static_cast<uint16_t>(read_reg_unsigned(params.rt) & 0xFFFF));
    break;
// TODO: handle endianness
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
