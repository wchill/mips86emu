#ifndef MIPS86CORE_COPROCESSOR_H
#define MIPS86CORE_COPROCESSOR_H

case OP_Z0:
case OP_Z1:
case OP_Z2:
case OP_Z3:
    bytes[0] = params.opcode - 0x10;
    switch(params.rs) {
        case OPZ_MFCZ:
            if(coprocessors[bytes[0]] == NULL) {
                throw std::runtime_error(fmt::sprintf("MFC%d failed: Coprocessor %d not installed", bytes[0], bytes[0]));
            }
            write_reg(params.rt, coprocessors[bytes[0]]->move_from_coprocessor(params));
            break;

        case OPZ_MTCZ:
            if(coprocessors[bytes[0]] == NULL) {
                throw std::runtime_error(fmt::sprintf("MTC%d failed: Coprocessor %d not installed", bytes[0], bytes[0]));
            }
            coprocessors[bytes[0]]->move_to_coprocessor(params, read_reg_unsigned(params.rt));
            break;

        case OPZ_CFCZ:
            if(coprocessors[bytes[0]] == NULL) {
                throw std::runtime_error(fmt::sprintf("CFC%d failed: Coprocessor %d not installed", bytes[0], bytes[0]));
            }
            write_reg(params.rt, coprocessors[bytes[0]]->move_from_coprocessor_control(params));
            break;

        case OPZ_CTCZ:
            if(coprocessors[bytes[0]] == NULL) {
                throw std::runtime_error(fmt::sprintf("CTC%d failed: Coprocessor %d not installed", bytes[0], bytes[0]));
            }
            coprocessors[bytes[0]]->move_to_coprocessor_control(params, read_reg_unsigned(params.rt));
            break;

        default:
            throw std::runtime_error("Unimplemented coprocessor instruction");
    }
    break;

case OP_LWC1:
    if(coprocessors[1] == NULL) {
        throw std::runtime_error("LWC1 failed: Coprocessor 1 not installed");
    }
    coprocessors[1]->load_word(params, read_reg_unsigned(params.rs));
    break;

case OP_SWC1:
    if(coprocessors[1] == NULL) {
        throw std::runtime_error("SWC1 failed: Coprocessor 1 not installed");
    }
    coprocessors[1]->store_word(params, read_reg_unsigned(params.rs));
    break;

case OP_LWC2:
    if(coprocessors[2] == NULL) {
        throw std::runtime_error("LWC2 failed: Coprocessor 2 not installed");
    }
    coprocessors[2]->load_word(params, read_reg_unsigned(params.rs));
    break;

case OP_SWC2:
    if(coprocessors[2] == NULL) {
        throw std::runtime_error("SWC2 failed: Coprocessor 2 not installed");
    }
    coprocessors[2]->store_word(params, read_reg_unsigned(params.rs));
    break;

#endif //MIPS86CORE_COPROCESSOR_H
