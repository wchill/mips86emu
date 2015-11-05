#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "mips.h"

#define INTERRUPT_PERIOD 10000
#define EMULATED_MEMORY 64 * 1024 * 1024
#define PAGE_SIZE 4096
#define NUM_PAGES (4294967296UL / PAGE_SIZE)
#define MEMORY_RESERVED 0
#define MEMORY_TEXT 0x04000000
#define MEMORY_STATIC 0x10000000
#define MEMORY_STACK 0x7FFFFFFF

uint8_t *memory;
uint8_t **memory_pages;
uint32_t pages_allocated;
uint32_t *registers;
uint32_t pc;

static inline uint8_t *physical_addr(register uint32_t virtual_addr) {
    return &memory_pages[virtual_addr >> 12][virtual_addr & 0xFFF];
}

uint32_t readWord(register uint32_t addr) {
    uint32_t page_num = (addr >> 12);
    if(!memory_pages[page_num]) {
        printf("\nMachine crash! Segmentation fault!\n");
        printf("Illegal access at address 0x%08x\n", addr);
        printf("PC: 0x%08x\n", pc);
        exit(1);
    }
    return *((uint32_t*)(&memory_pages[page_num][addr & 0xFFF]));
}

void writeMemory(register uint32_t addr, void *data, uint32_t len) {
    for(int i = 0; i < len; i += PAGE_SIZE) {
        uint32_t page_num = (addr >> 12);
        if(!memory_pages[page_num]) {
            memory_pages[page_num] = malloc(PAGE_SIZE);
            printf("Allocating 4kB page #%d at address 0x%08x, %d allocated total\n", page_num, addr + i, ++pages_allocated); 
        }
        if(len - i < PAGE_SIZE) {
            memcpy(memory_pages[page_num], &(((uint8_t*)data)[i]), len - i);
        } else {
            memcpy(memory_pages[page_num], &(((uint8_t*)data)[i]), PAGE_SIZE);
        }
    }
}

void writeWord(uint32_t addr, uint32_t word) {
    writeMemory(addr, &word, sizeof(word));
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

static inline void j_inst(uint32_t inst, uint32_t *address) {
    *address = (pc & 0xf0000000) | ((inst & 0x3ffffff) << 2);
}

static inline uint16_t __bswap_16(uint16_t x) {
    return (x>>8) | (x<<8);
}

static inline uint32_t __bswap_32(uint32_t x) {
    return (__bswap_16(x&0xffff)<<16) | (__bswap_16(x>>16));
}

int execute_syscall(uint32_t service) {
    switch(service) {
        // print integer
        case 1:
            printf("%d", registers[R_A0]);
            break;
        // print string
        case 4:
            printf("%s", physical_addr(registers[R_A0]));
            break;
        default:
            printf("Syscall 0x%02x unimplemented\n", service);
            return -1;
    }
    return 0;
}

void print_registers() {
    printf("Registers:\n");
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
    memory_pages = malloc(sizeof(*memory_pages) * NUM_PAGES);
    memset(memory_pages, 0, sizeof(*memory_pages) * NUM_PAGES);

    registers = malloc(sizeof(*registers) * 32);
    memset(registers, 0, 32 * sizeof(*registers));
    registers[R_SP] = (uint32_t) (EMULATED_MEMORY);

    fseek(file,0,SEEK_END);
    off_t file_size = ftell(file);
    fseek(file,0,SEEK_SET);
    void *buf = malloc(sizeof(uint8_t) * file_size);
    off_t bytesRead = fread(buf, 1, file_size, file);
    fclose(file);
    writeMemory(MEMORY_TEXT, buf, file_size);
    free(buf);

    uint32_t cycle_counter = INTERRUPT_PERIOD;
    // start of text segment
    pc = MEMORY_TEXT;

    int8_t exit_code = 0;

    bool emulator_running = true;
    while(emulator_running) {
        uint32_t instruction = __bswap_32(readWord(pc));
        //uint32_t instruction = *((uint32_t*) &(memory[pc]));
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
                        if(execute_syscall(registers[R_V0])) {
                            printf("Instruction: 0x%08x\n", instruction);
                        }
                    }
                    break;
                default:
                    printf("Secondary opcode 0x%02x unimplemented\n", funct);
                    printf("Instruction: 0x%08x\n", instruction);
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
                        //registers[rt] = memory[(int32_t) registers[rs] + (int16_t) immediate];
                        registers[rt] = readWord((int32_t) registers[rs] + (int16_t) immediate);
                        break;
                    case OP_SW:
                        //memory[(int32_t) registers[rs] + (int16_t) immediate] = registers[rt];
                        //registers[rt] = readWord((int32_t) registers[rs] + (int16_t) immediate);
                        writeWord((int32_t) registers[rs] + (int16_t) immediate, registers[rt]);
                        break;
                    default:
                        printf("Opcode 0x%02x unimplemented\n", opcode);
                        printf("Instruction: 0x%08x\n", instruction);
                }
            }
        }
    }

    cleanup:
    print_registers();
    for(int i = 0; i < NUM_PAGES; i++) {
        if(memory_pages[i]) {
            free(memory_pages[i]);
        }
    }
    free(memory_pages);
    free(registers);

    return exit_code;
}
