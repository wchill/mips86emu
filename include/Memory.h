//
// Created by Eric Ahn on 12/22/15.
//

#ifndef MIPS86CORE_MEMORY_H
#define MIPS86CORE_MEMORY_H

#include <cstdint>
#include <string>
#include "format.h"

#define ADDR_ALIGN_MASK 0xfffffffc
#define PAGE_SIZE 4096

class Memory {
private:
    uint8_t **memory_pages;
    unsigned int allocated_pages;
    unsigned int max_pages;

    unsigned int get_page_num(uint32_t virtual_addr);
    uint8_t *get_physical_addr(uint32_t virtual_addr);
    void allocate_page(unsigned int page);
    void memory_init(uint64_t max_memory);
public:
    Memory();
    Memory(uint64_t max_memory);
    ~Memory();

    uint32_t read_word(uint32_t addr);
    uint8_t read_byte(uint32_t addr);
    void read_memory(uint32_t addr, void *buf, uint32_t len);

    void write_word(uint32_t addr, uint32_t word);
    void write_byte(uint32_t addr, uint8_t data);
    void write_memory(uint32_t addr, void *buf, uint32_t len);
};


#endif //MIPS86CORE_MEMORY_H
