#ifndef MIPS86CORE_JUMP_REG_H
#define MIPS86CORE_JUMP_REG_H

case OP0_JALR:
    words[0] = pc + 8;
    tick();
    pc = read_reg_unsigned(params.rs) - 4;
    write_reg(params.rd, words[0]);
    break;

case OP0_JR:
    tick();
    pc = read_reg_unsigned(params.rs) - 4;
    break;

#endif //MIPS86CORE_JUMP_REG_H
