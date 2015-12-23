#ifndef MIPS86CORE_BRANCH_OTHER_H
#define MIPS86CORE_BRANCH_OTHER_H

case OP_OTHER1:
    switch(params.rt) {
        case OP1_BGEZ:
            if(read_reg_signed(params.rs) >= 0) {
                branch(params.immediate);
            }
            break;

        case OP1_BGEZAL:
            words[0] = pc;
            if(read_reg_signed(params.rs) >= 0) {
                branch(params.immediate);
            }
            if(words[0] != pc) {
                write_reg(R_RA, words[0] + 8);
            }
            break;

        case OP1_BLTZ:
            if(read_reg_signed(params.rs) < 0) {
                branch(params.immediate);
            }
            break;

        case OP1_BLTZAL:
            words[0] = pc;
            if(read_reg_signed(params.rs) < 0) {
                branch(params.immediate);
            }
            if(words[0] != pc) {
                write_reg(R_RA, words[0] + 8);
            }
            break;

        default:
            cerr << fmt::sprintf("Unimplemented OTHER1 instruction @ %#08x: %#02x", pc, params.rt) << endl;
            break;
    }
    break;

#endif //MIPS86CORE_BRANCH_OTHER_H
