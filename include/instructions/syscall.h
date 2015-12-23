#ifndef MIPS86CORE_SYSCALL_H
#define MIPS86CORE_SYSCALL_H
/*
void instr_syscall(r_inst params) {
    uint32_t syscall = read_reg_unsigned(R_V0);
    if(syscall == 17) {
        //exit_code = registers[R_A0];
    }
    if(syscall == 10 || syscall == 17) {
        emulator_running = false;
    } else {
        execute_syscall(syscall);
    }
}

void instr_break(r_inst params) {
    printf("BREAK at address 0x%08x\n", pc);
    printf("Press enter to continue...\n");
    getchar();
}

int execute_syscall(uint32_t service) {
    switch(service) {
        // print integer
        case 1:
            printf("%d", read_reg_unsigned(R_A0));
            break;
        // print string
        case 4:
            printf("%s", get_physical_addr(read_reg_unsigned(R_A0)));
            break;
        default:
            printf("Syscall 0x%02x unimplemented @ 0x%08x\n", service, pc);
            return -1;
    }
    return 0;
}
 */

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
    cout << "Press enter to continue..." << endl;
    cin.get();
    break;

#endif //MIPS86CORE_SYSCALL_H
