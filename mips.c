#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "mips.h"

#define INTERRUPT_PERIOD 10000
#define EMULATED_MEMORY 64 * 1024 * 1024

uint8_t *memory;
uint32_t *registers;

void execute_syscall(uint32_t service) {
    switch(service) {
        // print integer
        case 1:
            printf("%d", registers[R_A0]);
            break;
        // print string
        case 4:
            printf("%s", (char *) &(memory[registers[R_A0]]));
            break;
        default:
            printf("Syscall 0x%02x unimplemented\n", service);
    }
}

static inline void r_inst(uint32_t inst, uint8_t *rs, uint8_t *rt, uint8_t *rd, uint8_t *shamt, uint8_t *funct) {
    *rs = (uint8_t) ((inst >> 21) & 0b11111);
    *rt = (uint8_t) ((inst >> 16) & 0b11111);
    *rd = (uint8_t) ((inst >> 11) & 0b11111);
    *shamt = (uint8_t) ((inst >> 6) & 0b11111);
    *funct = (uint8_t) (inst & 0b111111);
}

static inline void i_inst(uint32_t inst, uint8_t *rs, uint8_t *rt, uint16_t *immediate) {
    *rs = (uint8_t) ((inst >> 21) & 0b11111);
    *rt = (uint8_t) ((inst >> 16) & 0b11111);
    *immediate = (uint16_t) (inst & 0xffff);
}

static inline void j_inst(uint32_t inst, uint32_t pc, uint32_t *address) {
    *address = (pc & 0xf0000000) | ((inst & 0x3ffffff) << 2);
}

static inline uint16_t __bswap_16(uint16_t x) {
    return (x>>8) | (x<<8);
}

static inline uint32_t __bswap_32(uint32_t x) {
    return (__bswap_16(x&0xffff)<<16) | (__bswap_16(x>>16));
}

void print_registers() {
    printf("$0  0x%08x   $t0 0x%08x   $s0 0x%08x   $t8 0x%08x\n", registers[R_ZERO], registers[R_T0], registers[R_S0], registers[R_T8]);
    printf("$at 0x%08x   $t1 0x%08x   $s1 0x%08x   $t9 0x%08x\n", registers[R_AT], registers[R_T1], registers[R_S1], registers[R_T9]);
    printf("$v0 0x%08x   $t2 0x%08x   $s2 0x%08x   $k0 0x%08x\n", registers[R_V0], registers[R_T2], registers[R_S2], registers[R_K0]);
    printf("$v1 0x%08x   $t3 0x%08x   $s3 0x%08x   $k1 0x%08x\n", registers[R_V1], registers[R_T3], registers[R_S3], registers[R_K1]);
    printf("$a0 0x%08x   $t4 0x%08x   $s4 0x%08x   $gp 0x%08x\n", registers[R_A0], registers[R_T4], registers[R_S4], registers[R_GP]);
    printf("$a1 0x%08x   $t5 0x%08x   $s5 0x%08x   $sp 0x%08x\n", registers[R_A1], registers[R_T5], registers[R_S5], registers[R_SP]);
    printf("$a2 0x%08x   $t6 0x%08x   $s6 0x%08x   $fp 0x%08x\n", registers[R_A2], registers[R_T6], registers[R_S6], registers[R_FP]);
    printf("$a3 0x%08x   $t7 0x%08x   $s7 0x%08x   $ra 0x%08x\n", registers[R_A3], registers[R_T7], registers[R_S7], registers[R_RA]);
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
    
    memory = malloc(sizeof(*memory) * EMULATED_MEMORY);
    memset(memory, 0, EMULATED_MEMORY);

    registers = malloc(sizeof(*registers) * 32);
    memset(registers, 0, 32 * sizeof(*registers));
    registers[R_SP] = (uint32_t) (EMULATED_MEMORY);

    fseek(file,0,SEEK_END);
    off_t file_size = ftell(file);
    fseek(file,0,SEEK_SET);
    off_t bytesRead = fread(memory + 0x00400000, 1, file_size, file);
    fclose(file);

    uint32_t cycle_counter = INTERRUPT_PERIOD;
    // start of text segment
    uint32_t pc = 0x00400000;

    int8_t exit_code = 0;

    bool emulator_running = true;
    while(emulator_running) {
        uint32_t instruction = __bswap_32(*((uint32_t*) &(memory[pc])));
        pc += 4;

        uint8_t opcode = (uint8_t) (instruction >> 26);

        // R type instruction
        if(opcode == OP_OTHER0) {
            uint8_t rs, rt, rd, shamt, funct;
            r_inst(instruction, &rs, &rt, &rd, &shamt, &funct);
            switch(funct) {
                case OP0_SLLV:
                    shamt = (uint8_t) (registers[rs] & 0b11111);
                case OP0_SLL:
                    registers[rd] = registers[rt] << shamt;
                    break;
                case OP0_SRLV:
                    shamt = (uint8_t) (registers[rs] & 0b11111);
                case OP0_SRL:
                    registers[rd] = registers[rt] >> shamt;
                    break;
                case OP0_SRAV:
                    shamt = (uint8_t) (registers[rs] & 0b11111);
                case OP0_SRA:
                    registers[rd] = (uint32_t) (((int32_t) registers[rt]) >> shamt);
                    break;
                case OP0_JALR:
                    registers[R_RA] = pc + 4;
                case OP0_JR:
                    pc = registers[rs];
                    break;
                case OP0_ADD:
                    registers[rd] = (uint32_t) ((int32_t) registers[rs] + (int32_t) registers[rt]);
                    break;
                case OP0_ADDU:
                    registers[rd] = registers[rs] + registers[rt];
                    break;
                case OP0_SUB:
                    registers[rd] = (uint32_t) ((int32_t) registers[rs] - (int32_t) registers[rt]);
                    break;
                case OP0_SUBU:
                    registers[rd] = registers[rs] - registers[rt];
                    break;
                case OP0_AND:
                    registers[rd] = registers[rs] & registers[rt];
                    break;
                case OP0_OR:
                    registers[rd] = registers[rs] | registers[rt];
                    break;
                case OP0_XOR:
                    registers[rd] = registers[rs] ^ registers[rt];
                    break;
                case OP0_NOR:
                    registers[rd] = ~(registers[rs] | registers[rt]);
                    break;
                case OP0_SYSCALL:
                    if(registers[R_V0] == 10 || registers[R_V0] == 17) {
                        if(registers[R_V0] == 17) {
                            exit_code = registers[R_A0];
                        }
                        emulator_running = false;
                    } else {
                        execute_syscall(registers[R_V0]);
                    }
                    break;
                default:
                    printf("Secondary opcode 0x%02x unimplemented\n", funct);
            }
        } else {
            if(opcode == OP_J || opcode == OP_JAL) {
                printf("Opcode 0x%02x unimplemented\n", opcode);
            } else {
                uint8_t rs, rt;
                uint16_t immediate;
                i_inst(instruction, &rs, &rt, &immediate);
                switch(opcode) {
                    case OP_LUI:
                        registers[rt] = ((uint32_t) immediate) << 16;
                        break;
                    case OP_ORI:
                        registers[rt] = registers[rs] | immediate;
                        break;
                    case OP_LW:
                        registers[rt] = memory[(int32_t) registers[rs] + (int16_t) immediate];
                        break;
                    case OP_SW:
                        memory[(int32_t) registers[rs] + (int16_t) immediate] = registers[rt];
                        break;
                    default:
                        printf("Opcode 0x%02x unimplemented\n", opcode);
                }
            }
        }
    }

    cleanup:
    printf("Registers:\n");
    print_registers();
    free(memory);
    free(registers);

    return exit_code;
}
