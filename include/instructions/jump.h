#ifndef MIPS86CORE_JUMP_H
#define MIPS86CORE_JUMP_H

case OP_JAL:
    execute(get_next_instruction());
    words[0] = pc + 4;
    pc = params.target - 4;
    write_reg(R_RA, words[0]);
    break;

case OP_J:
    execute(get_next_instruction());
    pc = params.target - 4;
    break;

#endif //MIPS86CORE_JUMP_H
