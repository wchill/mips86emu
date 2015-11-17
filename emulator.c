#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "mips.h"
#include "memory.h"
#include "register.h"
#include "instructions.h"

uint32_t pc = MEMORY_TEXT;
bool emulator_running = false;

void cleanup(int exit_code);

static inline uint32_t sign_extend_imm16(register uint16_t immediate) {
    return (uint32_t) (0xFFFF0000 * (immediate >> 15) + immediate);
}

static inline void i_inst(uint32_t inst, uint8_t *rs, uint8_t *rt, uint16_t *immediate) {
    *rs = (uint8_t) ((inst >> 21) & 0b11111);
    *rt = (uint8_t) ((inst >> 16) & 0b11111);
    *immediate = (uint16_t) (inst & 0xffff);
}

static inline uint32_t j_inst(register uint32_t inst) {
    return (pc & 0xf0000000) | ((inst & 0x3ffffff) << 2);
}

static inline uint16_t __bswap_16(uint16_t x) {
    return (x>>8) | (x<<8);
}

static inline uint32_t __bswap_32(uint32_t x) {
    return (__bswap_16(x&0xffff)<<16) | (__bswap_16(x>>16));
}

void print_registers() {
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
    // cleanup
    print_registers();
    memory_cleanup();
    //free(registers);
    printf("Exiting with code %d\n", exit_code);
    exit(exit_code);
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("Usage: mips86emu program.hex\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if(file == NULL) {
        printf("Error opening file: %s\n", argv[1]);
        return 1;
    }

    //registers = malloc(sizeof(*registers) * 32);
    //memset(registers, 0, 32 * sizeof(*registers));
    
    memory_init();
    write_reg(R_SP, EMULATED_MEMORY);
    //registers[R_SP] = (uint32_t) (EMULATED_MEMORY);

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
    // start of text segment
    // temporarily set to 0x400018 until I figure out the ELF format
    pc = MEMORY_TEXT + 24;

    int8_t exit_code = 0;
    emulator_running = true;

    while(emulator_running) {
        //convert big endian to little endian
        uint32_t instruction = __bswap_32(read_word(pc));
        //printf("Executing instruction 0x%08x @ 0x%08x\n", instruction, pc);
        //uint32_t instruction = readWord(pc);

        uint8_t opcode = (uint8_t) (instruction >> 26);

        // R type instruction
        if(opcode == OP_OTHER0) {
            r_inst params;
            parse_r_inst(instruction, &params);
            r_inst_table[params.funct](params);
        } else if(opcode == OP_J || opcode == OP_JAL) {
            uint32_t imm_addr = j_inst(instruction);
            if(opcode == OP_JAL) {
                write_reg(R_RA, pc);
            }
            pc = imm_addr;
        } else {
            uint8_t rs, rt;
            uint16_t immediate;
            i_inst(instruction, &rs, &rt, &immediate);
            switch(opcode) {
                case OP_ADDIU:
                    write_reg(rt, read_reg_unsigned(rs) + sign_extend_imm16(immediate));
                    break;
                case OP_LUI:
                    write_reg(rt, ((uint32_t) immediate) << 16);
                    break;
                case OP_ORI:
                    write_reg(rt, read_reg_unsigned(rs) | immediate);
                    break;
                case OP_LW:
                    //registers[rt] = memory[(int32_t) registers[rs] + (int16_t) immediate];
                    write_reg(rt, read_word(read_reg_signed(rs) + (int16_t) immediate));
                    break;
                case OP_SW:
                    //memory[(int32_t) registers[rs] + (int16_t) immediate] = registers[rt];
                    //registers[rt] = readWord((int32_t) registers[rs] + (int16_t) immediate);
                    write_word(read_reg_signed(rs) + (int16_t) immediate, read_reg_unsigned(rt));
                    break;
                default:
                    printf("Opcode 0x%02x unimplemented\n", opcode);
                    printf("Instruction: 0x%08x\n", instruction);
            }
        }
        pc += 4;
    }

    cleanup(exit_code);
}
