#include <stdio.h>
#include "inst_r.h"
#include "register.h"
#include "memory.h"

void parse_r_inst(uint32_t inst, r_inst *params) {
    params->instruction = inst;
    params->rs = (uint8_t) ((inst >> 21) & 0b11111);
    params->rt = (uint8_t) ((inst >> 16) & 0b11111);
    params->rd = (uint8_t) ((inst >> 11) & 0b11111);
    params->shamt = (uint8_t) ((inst >> 6) & 0b11111);
    params->funct = (uint8_t) (inst & 0b111111);
}

void instr_shift_left_logical(r_inst params) {
    write_reg(params.rd, read_reg_unsigned(params.rt) << params.shamt);
}

void instr_shift_left_logical_variable(r_inst params) {
    params.shamt = (uint8_t) (read_reg_unsigned(params.rs) & 0b11111);
    instr_shift_left_logical(params);
}

void instr_shift_right_logical(r_inst params) {
    write_reg(params.rd, read_reg_unsigned(params.rt) >> params.shamt);
}

void instr_shift_right_logical_variable(r_inst params) {
    params.shamt = (uint8_t) (read_reg_unsigned(params.rs) & 0b11111);
    instr_shift_right_logical(params);
}

void instr_shift_right_arith(r_inst params) {
    write_reg(params.rd, (read_reg_signed(params.rt) >> params.shamt));
}

void instr_shift_right_arith_variable(r_inst params) {
    params.shamt = (uint8_t) (read_reg_unsigned(params.rs) & 0b11111);
    instr_shift_right_arith(params);
}

void instr_add(r_inst params) {
    write_reg(params.rd, read_reg_signed(params.rs) + read_reg_signed(params.rt));
}

void instr_add_unsigned(r_inst params) {
    write_reg(params.rd, read_reg_unsigned(params.rs) + read_reg_unsigned(params.rt));
}

void instr_sub(r_inst params) {
    write_reg(params.rd, read_reg_signed(params.rs) - read_reg_signed(params.rt));
}

void instr_sub_unsigned(r_inst params) {
    write_reg(params.rd, read_reg_unsigned(params.rs) - read_reg_unsigned(params.rt));
}

void instr_and(r_inst params) {
    write_reg(params.rd, read_reg_unsigned(params.rs) & read_reg_unsigned(params.rt));
}

void instr_or(r_inst params) {
    write_reg(params.rd, read_reg_unsigned(params.rs) | read_reg_unsigned(params.rt));
}

void instr_xor(r_inst params) {
    write_reg(params.rd, read_reg_unsigned(params.rs) ^ read_reg_unsigned(params.rt));
}

void instr_nor(r_inst params) {
    write_reg(params.rd, ~(read_reg_unsigned(params.rs) | read_reg_unsigned(params.rt)));
}

void instr_move_from_hi(r_inst params) {
    write_reg(params.rd, read_hi_reg());
}

void instr_move_to_hi(r_inst params) {
    write_hi_reg(read_reg_unsigned(params.rs));
}

void instr_move_from_lo(r_inst params) {
    write_reg(params.rd, read_lo_reg());
}

void instr_move_to_lo(r_inst params) {
    write_lo_reg(read_reg_unsigned(params.rs));
}

void instr_multiply(r_inst params) {
    uint64_t dword = (uint64_t) ((int64_t) read_reg_signed(params.rs)) * ((int64_t) read_reg_signed(params.rt));
    write_hi_reg((uint32_t) (dword >> 32));
    write_lo_reg((uint32_t) (dword & 0xFFFFFFFF));
}

void instr_multiply_unsigned(r_inst params) {
    uint64_t dword = ((uint64_t) read_reg_unsigned(params.rs)) * ((uint64_t) read_reg_unsigned(params.rt));
    write_hi_reg((uint32_t) (dword >> 32));
    write_lo_reg((uint32_t) (dword & 0xFFFFFFFF));
}

void instr_divide(r_inst params) {
    uint32_t quotient = (uint32_t) (read_reg_signed(params.rs) / read_reg_signed(params.rt));
    uint32_t remainder = (uint32_t) (read_reg_signed(params.rs) % read_reg_signed(params.rt));
    write_lo_reg(quotient);
    write_hi_reg(remainder);
}

void instr_divide_unsigned(r_inst params) {
    uint32_t quotient = (uint32_t) (read_reg_unsigned(params.rs) / read_reg_unsigned(params.rt));
    uint32_t remainder = (uint32_t) (read_reg_unsigned(params.rs) % read_reg_unsigned(params.rt));
    write_lo_reg(quotient);
    write_hi_reg(remainder);
}

void instr_set_less_than(r_inst params) {
    write_reg(params.rd, read_reg_signed(params.rs) < read_reg_signed(params.rt));
}

void instr_set_less_than_unsigned(r_inst params) {
    write_reg(params.rd, read_reg_unsigned(params.rs) < read_reg_unsigned(params.rt));
}

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

void instr_unimplemented_r(r_inst params) {
    printf("Secondary opcode 0x%02x unimplemented\n", params.funct);
    printf("R-type instruction: 0x%08x @ 0x%08x\n", params.instruction, pc);
}

void instr_jump_and_link_reg(r_inst params) {
    uint32_t old_pc = pc + 4;
    instr_jump_reg(params);
    write_reg(R_RA, old_pc);
}

void instr_jump_reg(r_inst params) {
    pc = read_reg_unsigned(params.rs);
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

void (*r_inst_table[])(r_inst) = {
    instr_shift_left_logical,               // 0x00
    instr_unimplemented_r,
    instr_shift_right_logical,              // 0x02
    instr_shift_right_arith,                // 0x03
    instr_shift_left_logical_variable,      // 0x04
    instr_unimplemented_r,
    instr_shift_right_logical_variable,     // 0x06
    instr_shift_right_arith_variable,       // 0x07
    instr_jump_reg,                         // 0x08
    instr_jump_and_link_reg,                // 0x09
    instr_unimplemented_r,
    instr_unimplemented_r,
    instr_syscall,                          // 0x0c
    instr_break,                            // 0x0d
    instr_unimplemented_r,
    instr_unimplemented_r,
    instr_move_from_hi,                     // 0x10
    instr_move_to_hi,                       // 0x11
    instr_move_from_lo,                     // 0x12
    instr_move_to_lo,                       // 0x13
    instr_unimplemented_r,
    instr_unimplemented_r,
    instr_unimplemented_r,
    instr_unimplemented_r,
    instr_multiply,                         // 0x18
    instr_multiply_unsigned,                // 0x19
    instr_divide,                           // 0x1a
    instr_divide_unsigned,                  // 0x1b
    instr_unimplemented_r,
    instr_unimplemented_r,
    instr_unimplemented_r,
    instr_unimplemented_r,
    instr_add,                              // 0x20
    instr_add_unsigned,                     // 0x21
    instr_sub,                              // 0x22
    instr_sub_unsigned,                     // 0x23
    instr_and,                              // 0x24
    instr_or,                               // 0x25
    instr_xor,                              // 0x26
    instr_nor,                              // 0x27
    instr_unimplemented_r,
    instr_unimplemented_r,
    instr_set_less_than,                    // 0x2a
    instr_set_less_than_unsigned            // 0x2b
};
