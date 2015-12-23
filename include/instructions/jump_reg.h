#ifndef MIPS86CORE_JUMP_REG_H
#define MIPS86CORE_JUMP_REG_H

case OP0_JALR:
    execute(get_next_instruction());
    words[0] = pc + 4;
    pc = read_reg_unsigned(params.rs) - 4;
    write_reg(R_RA, words[0]);
    break;

case OP0_JR:
    execute(get_next_instruction());
    pc = read_reg_unsigned(params.rs) - 4;
    break;

#endif //MIPS86CORE_JUMP_REG_H
