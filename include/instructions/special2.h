#ifndef MIPS86CORE_SPECIAL2_H
#define MIPS86CORE_SPECIAL2_H

case OP_SPECIAL2:
    switch (params.funct) {

        case OP2_CLZ:
            words[0] = read_reg_unsigned(params.rs);
            words[2] = 32;

            words[1] = words[0] >>16;  if (words[1] != 0) {words[2] = words[2] -16;  words[0] = words[1];}
            words[1] = words[0] >> 8;  if (words[1] != 0) {words[2] = words[2] - 8;  words[0] = words[1];}
            words[1] = words[0] >> 4;  if (words[1] != 0) {words[2] = words[2] - 4;  words[0] = words[1];}
            words[1] = words[0] >> 2;  if (words[1] != 0) {words[2] = words[2] - 2;  words[0] = words[1];}
            words[1] = words[0] >> 1;  if (words[1] != 0) words[0] = 2;
            write_reg(params.rt, words[2] - words[0]);
            break;

        case OP2_CLO:
            words[0] = read_reg_unsigned(params.rs);
            words[0] -= ((words[0] >> 1) & 0x55555555);
            words[0] = (((words[0] >> 2) & 0x33333333) + (words[0] & 0x33333333));
            words[0] = (((words[0] >> 4) + words[0]) & 0x0f0f0f0f);
            words[0] += (words[0] >> 8);
            words[0] += (words[0] >> 16);
            write_reg(params.rt, words[0] & 0x0000003f);
            break;

    }
    break;

#endif //MIPS86CORE_SPECIAL2_H
