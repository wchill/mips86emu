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
    words_s[0] = read_reg_signed(params.rs);
    words_s[1] = read_reg_signed(params.rt);
    if(words_s[1] == 0) throw CpuException(instruction, pc);
    dwords_s[0] = static_cast<int64_t>(words_s[0]) / static_cast<int64_t>(words_s[1]);
    dwords_s[1] = static_cast<int64_t>(words_s[0]) % static_cast<int64_t>(words_s[1]);
    if(dwords_s[0] == 2147483648) {
        cerr << fmt::sprintf("Warning: signed division overflow @ %#08x", pc) << endl;
        dwords_s[0] = 0;
    }
    write_lo(static_cast<int32_t>(dwords_s[0]));
    write_hi(static_cast<int32_t>(dwords_s[1]));
    break;

case OP0_DIVU:
    words[0] = read_reg_unsigned(params.rs);
    words[1] = read_reg_unsigned(params.rt);
    if(words[1] == 0) throw ArithmeticException(instruction, pc);
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
