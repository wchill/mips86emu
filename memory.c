#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "memory.h"

void memory_init() {
    memory_pages = malloc(sizeof(*memory_pages) * NUM_PAGES);
    memset(memory_pages, 0, sizeof(*memory_pages) * NUM_PAGES);
}

void memory_cleanup() {
    for(int i = 0; i < NUM_PAGES; i++) {
        if(memory_pages[i]) {
            free(memory_pages[i]);
        }
    }
    free(memory_pages);
}

void allocate_page(uint32_t page_num) {
    if(!memory_pages[page_num]) {
        printf("Allocating new page %d\n", page_num);
        memory_pages[page_num] = malloc(PAGE_SIZE);
    }
}

uint32_t read_word(uint32_t addr) {
    uint32_t page_num = get_page_num(addr);
    uint16_t offset = addr & 0xFFF;
    if(page_num > NUM_PAGES || !memory_pages[page_num]) {
        printf("\nMachine crash! Segmentation fault!\n");
        printf("Illegal access at address 0x%08x\n", addr);
        printf("PC: 0x%08x\n", pc);
        //cleanup(1);
    } else if (addr & ~ADDR_ALIGN_MASK) {
        printf("\nUnaligned word read at address 0x%08x\n", addr);
        printf("PC: 0x%08x\n", pc);
        //cleanup(1);
    }
    return *((uint32_t*)(&memory_pages[page_num][offset]));
}

void write_word(uint32_t addr, uint32_t word) {
    if(addr & ~ADDR_ALIGN_MASK) {
        printf("\nUnaligned word write at address 0x%08x\n", addr);
        printf("PC: 0x%08x\n", pc);
        //cleanup(1);
    }
    uint32_t page_num = get_page_num(addr);
    uint16_t offset = (addr & 0xFFF);
    allocate_page(page_num);
    *((uint32_t*)&(memory_pages[page_num][offset])) = word;
}

uint8_t read_byte(uint32_t addr) {
    uint32_t page_num = get_page_num(addr);
    uint16_t offset = addr & 0xFFF;
    if(page_num > NUM_PAGES || !memory_pages[page_num]) {
        printf("\nMachine crash! Segmentation fault!\n");
        printf("Illegal access at address 0x%08x\n", addr);
        printf("PC: 0x%08x\n", pc);
        //cleanup(1);
    }
    return *((uint8_t*)(&memory_pages[page_num][offset]));
}

void write_byte(uint32_t addr, uint8_t data) {
    uint32_t page_num = get_page_num(addr);
    uint16_t offset = (addr & 0xFFF);
    allocate_page(page_num);
    *((uint8_t*)(&memory_pages[page_num][offset])) = data;
}

void write_memory(uint32_t addr, void *data, uint32_t len) {
    for(int i = 0; i < len; i += PAGE_SIZE) {
        uint32_t page_num = get_page_num(addr + i);
        allocate_page(page_num);
        if(len - i < PAGE_SIZE) {
            memcpy(memory_pages[page_num], &(((uint8_t*)data)[i]), len - i);
        } else {
            memcpy(memory_pages[page_num], &(((uint8_t*)data)[i]), PAGE_SIZE);
        }
    }
}
