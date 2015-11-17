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
#define MEMORY_TEXT 0x00400000
#define MEMORY_STATIC 0x10000000
#define MEMORY_STACK 0x7FFFFFFF

uint8_t **memory_pages;
uint32_t pages_allocated;
uint32_t *registers;
uint32_t register_hi;
uint32_t register_lo;
uint32_t pc;

void cleanup(int exit_code);

static inline uint32_t sign_extend_imm16(register uint16_t immediate) {
    return (uint32_t) (0xFFFF0000 * (immediate >> 15) + immediate);
}

static inline uint32_t virtual_addr_to_page_num(register uint32_t virtual_addr) {
    return virtual_addr >> 12;
}

static inline uint8_t *physical_addr(register uint32_t virtual_addr) {
    return &(memory_pages[virtual_addr_to_page_num(virtual_addr)][virtual_addr & 0xFFF]);
}

static inline uint32_t read_reg(register uint8_t reg) {
    return registers[reg];
}

static inline int32_t read_reg_signed(register uint8_t reg) {
    return (int32_t) registers[reg];
}

static inline uint32_t read_reg_unsigned(register uint8_t reg) {
    return (uint32_t) registers[reg];
}

static inline uint32_t read_hi_reg() {
    return register_hi;
}

static inline uint32_t read_lo_reg() {
    return register_lo;
}

static inline void write_hi_reg(register uint32_t word) {
    register_hi = word;
}

static inline void write_lo_reg(register uint32_t word) {
    register_lo = word;
}

static inline void write_reg(register uint8_t reg, register uint32_t word) {
    if(reg) {
        registers[reg] = word;
    }
}

uint32_t readWord(register uint32_t addr) {
    uint32_t page_num = virtual_addr_to_page_num(addr);
    uint16_t offset = addr & 0xFFF;
    if(page_num > NUM_PAGES || !memory_pages[page_num]) {
        printf("\nMachine crash! Segmentation fault!\n");
        printf("Illegal access at address 0x%08x\n", addr);
        printf("PC: 0x%08x\n", pc);
        cleanup(1);
    } else if (addr & ~ADDR_ALIGN_MASK) {
        printf("\nUnaligned word read at address 0x%08x\n", addr);
        printf("PC: 0x%08x\n", pc);
        cleanup(1);
    }
    return *((uint32_t*)(&memory_pages[page_num][offset]));
}

static inline void allocate_page(uint32_t page_num) {
    if(!memory_pages[page_num]) {
        printf("Allocating new page %d\n", page_num);
        memory_pages[page_num] = malloc(PAGE_SIZE);
    }
}

void writeMemory(register uint32_t addr, register void *data, register uint32_t len) {
    for(int i = 0; i < len; i += PAGE_SIZE) {
        uint32_t page_num = virtual_addr_to_page_num(addr + i);
        allocate_page(page_num);
        if(len - i < PAGE_SIZE) {
            memcpy(memory_pages[page_num], &(((uint8_t*)data)[i]), len - i);
        } else {
            memcpy(memory_pages[page_num], &(((uint8_t*)data)[i]), PAGE_SIZE);
        }
    }
}

void writeWord(register uint32_t addr, register uint32_t word) {
    if(addr & ~ADDR_ALIGN_MASK) {
        printf("\nUnaligned word write at address 0x%08x\n", addr);
        printf("PC: 0x%08x\n", pc);
        cleanup(1);
    }
    uint32_t page_num = virtual_addr_to_page_num(addr);
    uint16_t offset = (addr & 0xFFF);
    allocate_page(page_num);
    *((uint32_t*)&(memory_pages[page_num][offset])) = word;
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

static inline uint32_t j_inst(register uint32_t inst) {
    return (pc & 0xf0000000) | ((inst & 0x3ffffff) << 2);
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

void cleanup(int exit_code) {
    // cleanup
    print_registers();
    for(int i = 0; i < NUM_PAGES; i++) {
        if(memory_pages[i]) {
            free(memory_pages[i]);
        }
    }
    free(memory_pages);
    free(registers);
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
    memory_pages = malloc(sizeof(*memory_pages) * NUM_PAGES);
    memset(memory_pages, 0, sizeof(*memory_pages) * NUM_PAGES);

    registers = malloc(sizeof(*registers) * 32);
    memset(registers, 0, 32 * sizeof(*registers));
    registers[R_SP] = (uint32_t) (EMULATED_MEMORY);

    fseek(file,0,SEEK_END);
    off_t file_size = ftell(file);
    fseek(file,0,SEEK_SET);
    void *buf = malloc(sizeof(uint8_t) * (file_size));
    fread(buf, 1, file_size, file);
    fclose(file);
    writeMemory(MEMORY_TEXT, buf, file_size);
    free(buf);
    printf("Program loaded (%lld bytes)\n", file_size);

    // uint32_t cycle_counter = INTERRUPT_PERIOD;
    // start of text segment
    // temporarily set to 0x400018 until I figure out the ELF format
    pc = MEMORY_TEXT + 24;

    int8_t exit_code = 0;

    bool emulator_running = true;
    while(emulator_running) {
        //convert big endian to little endian
        uint32_t instruction = __bswap_32(readWord(pc));
        //printf("Executing instruction 0x%08x @ 0x%08x\n", instruction, pc);
        //uint32_t instruction = readWord(pc);

        uint8_t opcode = (uint8_t) (instruction >> 26);

        // R type instruction
        if(opcode == OP_OTHER0) {
            uint8_t rs, rt, rd, shamt, funct;
            r_inst(instruction, &rs, &rt, &rd, &shamt, &funct);
            switch(funct) {
                case OP0_SLLV:
                    shamt = (uint8_t) (read_reg(rs) & 0b11111);
                case OP0_SLL:
                    write_reg(rd, read_reg(rt) << shamt);
                    break;
                case OP0_SRLV:
                    shamt = (uint8_t) (read_reg(rs) & 0b11111);
                case OP0_SRL:
                    write_reg(rd, read_reg(rt) >> shamt);
                    break;
                case OP0_SRAV:
                    shamt = (uint8_t) (read_reg(rs) & 0b11111);
                case OP0_SRA:
                    write_reg(rd, (uint32_t) (((int32_t) read_reg(rt)) >> shamt));
                    break;
                case OP0_JALR:
                    write_reg(R_RA, pc + 4);
                case OP0_JR:
                    pc = read_reg(rs);
                    break;
                case OP0_ADD:
                    write_reg(rd, (uint32_t) ((int32_t) registers[rs] + (int32_t) registers[rt]));
                    break;
                case OP0_ADDU:
                    write_reg(rd, read_reg(rs) + read_reg(rt));
                    break;
                case OP0_SUB:
                    write_reg(rd, (uint32_t) ((int32_t) registers[rs] - (int32_t) registers[rt]));
                    break;
                case OP0_SUBU:
                    write_reg(rd, read_reg(rs) - read_reg(rt));
                    break;
                case OP0_AND:
                    write_reg(rd, read_reg(rs) & read_reg(rt));
                    break;
                case OP0_OR:
                    write_reg(rd, read_reg(rs) | read_reg(rt));
                    break;
                case OP0_XOR:
                    write_reg(rd, read_reg(rs) ^ read_reg(rt));
                    break;
                case OP0_NOR:
                    write_reg(rd, ~(read_reg(rs) | read_reg(rt)));
                    break;
                case OP0_MFHI:
                    write_reg(rd, read_hi_reg());
                    break;
                case OP0_MFLO:
                    write_reg(rd, read_lo_reg());
                    break;
                case OP0_MTHI:
                    write_hi_reg(read_reg(rs));
                    break;
                case OP0_MTLO:
                    write_lo_reg(read_reg(rs));
                    break;
/*
                case OP0_MULT:
                    uint64_t dword_one = (uint64_t) (((int64_t) read_reg(rs)) * ((int64_t) read_reg(rt)));
                    write_hi_reg((uint32_t) (dword_one >> 32));
                    write_lo_reg((uint32_t) (dword_one & 0xFFFFFFFF));
                    break;
                case OP0_MULTU:
                    uint64_t dword_two = ((uint64_t) read_reg(rs)) * ((uint64_t) read_reg(rt));
                    write_hi_reg((uint32_t) (dword_two >> 32));
                    write_lo_reg((uint32_t) (dword_two & 0xFFFFFFFF));
                    break;
                case OP0_DIV:
                    int32_t quotient_signed = ((int32_t) read_reg(rs)) / ((int32_t) read_reg(rt));
                    int32_t remainder_signed = ((int32_t) read_reg(rs)) % ((int32_t) read_reg(rt));
                    write_lo_reg((uint32_t) quotient_signed);
                    write_hi_reg((uint32_t) remainder_signed);
                    break;
                case OP0_DIVU:
                    uint32_t quotient = ((uint32_t) read_reg(rs)) / ((uint32_t) read_reg(rt));
                    uint32_t remainder = ((uint32_t) read_reg(rs)) % ((uint32_t) read_reg(rt));
                    write_lo_reg((uint32_t) quotient);
                    write_hi_reg((uint32_t) remainder);
                    break;
*/
                case OP0_SLT:
                    write_reg(rd, ((int32_t) read_reg(rs)) < ((int32_t) read_reg(rt)));
                    break;
                case OP0_SLTU:
                    write_reg(rd, ((uint32_t) read_reg(rs)) < ((uint32_t) read_reg(rt)));
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
                case OP0_BREAK:
                    printf("BREAK at address 0x%08x\n", pc);
                    printf("Press enter to continue...\n");
                    getchar();
                default:
                    printf("Secondary opcode 0x%02x unimplemented\n", funct);
                    printf("Instruction: 0x%08x\n", instruction);
            }
        } else {
            if(opcode == OP_J || opcode == OP_JAL) {
                uint32_t imm_addr = j_inst(instruction);
                if(opcode == OP_JAL) {
                    write_reg(R_RA, pc);
                }
                pc = imm_addr;
                // Should a no-op be inserted here?
                // printf("Opcode 0x%02x unimplemented\n", opcode);
            } else {
                uint8_t rs, rt;
                uint16_t immediate;
                i_inst(instruction, &rs, &rt, &immediate);
                switch(opcode) {
                    case OP_ADDIU:
                        write_reg(rt, read_reg(rs) + sign_extend_imm16(immediate));
                        break;
                    case OP_LUI:
                        write_reg(rt, ((uint32_t) immediate) << 16);
                        break;
                    case OP_ORI:
                        write_reg(rt, read_reg(rs) | immediate);
                        break;
                    case OP_LW:
                        //registers[rt] = memory[(int32_t) registers[rs] + (int16_t) immediate];
                        write_reg(rt, readWord((int32_t) read_reg(rs) + (int16_t) immediate));
                        break;
                    case OP_SW:
                        //memory[(int32_t) registers[rs] + (int16_t) immediate] = registers[rt];
                        //registers[rt] = readWord((int32_t) registers[rs] + (int16_t) immediate);
                        writeWord((int32_t) read_reg(rs) + (int16_t) immediate, read_reg(rt));
                        break;
                    default:
                        printf("Opcode 0x%02x unimplemented\n", opcode);
                        printf("Instruction: 0x%08x\n", instruction);
                }
            }
        }
        pc += 4;
    }

    cleanup(exit_code);
}
