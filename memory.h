#ifndef MIPS_MEMORY
#define MIPS_MEMORY

#include <stdint.h>
#include "mips.h"

uint8_t **memory_pages;
uint32_t pages_allocated;

void memory_init();
void memory_cleanup();

static inline uint32_t get_page_num(uint32_t virtual_addr) {
    return virtual_addr >> 12;
}

static inline uint8_t *get_physical_addr(uint32_t virtual_addr) {
    return &(memory_pages[get_page_num(virtual_addr)][virtual_addr & 0xFFF]);
}

void allocate_page(uint32_t page);
uint32_t read_word(uint32_t addr);
void write_word(uint32_t addr, uint32_t word);
uint8_t read_byte(uint32_t addr);
void write_byte(uint32_t addr, uint8_t data);
void write_memory(uint32_t addr, void *data, uint32_t len);

#endif
