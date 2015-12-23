#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "mips.h"
#include "register.h"
#include "instructions.h"
#include "cop0.h"
#include "cop1.h"

uint32_t pc = MEMORY_TEXT;
bool emulator_running = false;
bool branch_taken = false;

void cleanup(int exit_code);

static inline uint16_t __bswap_16(uint16_t x) {
    return (x>>8) | (x<<8);
}

static inline uint32_t __bswap_32(uint32_t x) {
    return (__bswap_16(x&0xffff)<<16) | (__bswap_16(x>>16));
}

void print_registers() {
    uint32_t registers[32];
    int i;
    for(i = 0; i < 32; i++) {
        registers[i] = read_reg_unsigned(i);
    }
    /*
    printf("Registers:\n");
    printf("$0  0x%08x   $t0 0x%08x   $s0 0x%08x   $t8 0x%08x\n", registers[R_ZERO], registers[R_T0], registers[R_S0], registers[R_T8]);
    printf("$at 0x%08x   $t1 0x%08x   $s1 0x%08x   $t9 0x%08x\n", registers[R_AT], registers[R_T1], registers[R_S1], registers[R_T9]);
    printf("$v0 0x%08x   $t2 0x%08x   $s2 0x%08x   $k0 0x%08x\n", registers[R_V0], registers[R_T2], registers[R_S2], registers[R_K0]);
    printf("$v1 0x%08x   $t3 0x%08x   $s3 0x%08x   $k1 0x%08x\n", registers[R_V1], registers[R_T3], registers[R_S3], registers[R_K1]);
    printf("$a0 0x%08x   $t4 0x%08x   $s4 0x%08x   $gp 0x%08x\n", registers[R_A0], registers[R_T4], registers[R_S4], registers[R_GP]);
    printf("$a1 0x%08x   $t5 0x%08x   $s5 0x%08x   $sp 0x%08x\n", registers[R_A1], registers[R_T5], registers[R_S5], registers[R_SP]);
    printf("$a2 0x%08x   $t6 0x%08x   $s6 0x%08x   $fp 0x%08x\n", registers[R_A2], registers[R_T6], registers[R_S6], registers[R_FP]);
    printf("$a3 0x%08x   $t7 0x%08x   $s7 0x%08x   $ra 0x%08x\n", registers[R_A3], registers[R_T7], registers[R_S7], registers[R_RA]);
    */
}

void cleanup(int exit_code) {
    /*
    // cleanup
    print_registers();
    cop0_cleanup();
    cop1_cleanup();
    //free(registers);
    printf("Exiting with code %d\n", exit_code);
    exit(exit_code);
     */
}

int main(int argc, char *argv[]) {
    /*
    if(argc < 2) {
        printf("Usage: mips86emu program.hex\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if(file == NULL) {
        printf("Error opening file: %s\n", argv[1]);
        return 1;
    }

    write_reg(R_SP, STACK_TOP);
    write_reg(R_GP, DYNAMIC_BOTTOM);

    cop0_init();
    cop1_init();

    fseek(file,0,SEEK_END);
    off_t file_size = ftell(file);
    fseek(file,0,SEEK_SET);
    void *buf = malloc(sizeof(uint8_t) * (file_size));
    fread(buf, 1, file_size, file);
    fclose(file);
    write_memory(MEMORY_TEXT, buf, file_size);
    free(buf);
    printf("Program loaded (%lld bytes)\n", file_size);

    // uint32_t cycle_counter = INTERRUPT_PERIOD;


    int8_t exit_code = 0;
    emulator_running = true;

    while(emulator_running) {
        branch_taken = false;
        //convert big endian to little endian
        uint32_t instruction = __bswap_32(read_word(pc));
        //uint32_t instruction = read_word(pc);

        uint8_t opcode = (uint8_t) (instruction >> 26);

        // R type instruction
        printf("Executing instruction 0x%08x at 0x%08x\n", instruction, pc);
        if(opcode == OP_OTHER0) {
            r_inst params;
            parse_r_inst(instruction, &params);
            (*r_inst_table[params.funct])(params);
        } else if(opcode == OP_SPECIAL2) {
            r_inst params;
            parse_r_inst(instruction, &params);
            (*special_two_inst_table[params.funct])(params);
        } else if(opcode == OP_J || opcode == OP_JAL) {
            uint32_t imm_addr = j_inst(instruction);
            if(opcode == OP_JAL) {
                write_reg(R_RA, pc + 4);
            }
            pc = imm_addr;
            branch_taken = true;
        } else {
            i_inst params;
            parse_i_inst(instruction, &params);
            (*i_inst_table[params.opcode])(params);
        }
        pc += 4 * !branch_taken;
    }

    cleanup(exit_code);
     */
}