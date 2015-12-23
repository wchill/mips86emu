#ifndef MIPS86CORE_BRANCH_H
#define MIPS86CORE_BRANCH_H

case OP_BEQ:
    if(read_reg_unsigned(params.rs) == read_reg_unsigned(params.rt)) {
        branch(params.immediate);
    }
    break;

case OP_BNE:
    if(read_reg_unsigned(params.rs) != read_reg_unsigned(params.rt)) {
        branch(params.immediate);
    }
    break;

case OP_BLEZ:
    if(read_reg_signed(params.rs) <= 0) {
        branch(params.immediate);
    }
    break;

case OP_BGTZ:
    if(read_reg_signed(params.rs) > 0) {
        branch(params.immediate);
    }
    break;

#endif //MIPS86CORE_BRANCH_H
