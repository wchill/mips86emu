#ifndef MIPS86CORE_SYSCALL_H
#define MIPS86CORE_SYSCALL_H

// TODO: implement correct exiting procedure using exceptions
case OP0_SYSCALL:
    switch(read_reg_unsigned(R_V0)) {
        case 17:
        case 10:
            exit(0);
            break;
        case 1:
            cout << read_reg_unsigned(R_A0);
            break;
        case 4:
            cout << memory->get_physical_addr(read_reg_unsigned(R_A0));
            break;
        default:
            cerr << fmt::sprintf("Syscall %d not implemented", read_reg_unsigned(R_V0)) << endl;
            break;
    }
    break;

// TODO: Exception codes for BREAK
case OP0_BREAK:
    cout << fmt::sprintf("BREAK @ %#08x", pc) << endl;
    //cout << "Press enter to continue..." << endl;
    //cin.get();
    break;

#endif //MIPS86CORE_SYSCALL_H
