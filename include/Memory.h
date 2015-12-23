#ifndef MIPS86CORE_MEMORY_H
#define MIPS86CORE_MEMORY_H

#include <cstdint>
#include <string>
#include "format.h"

#define STACK_TOP 0x7FFFFFFC
#define DYNAMIC_BOTTOM 0x10008000
#define MEMORY_RESERVED 0
#define MEMORY_TEXT 0x00400000
#define MEMORY_STATIC 0x10000000
#define ADDR_ALIGN_MASK 0xfffffffc
#define PAGE_SIZE 4096

class Memory {
private:
    uint8_t **memory_pages;
    unsigned int allocated_pages;
    unsigned int max_pages;

    void memory_init(uint64_t max_memory);
    unsigned int get_page_num(uint32_t virtual_addr);
    void allocate_page(unsigned int page);
public:
    Memory();
    Memory(uint64_t max_memory);
    ~Memory();

    void clear_memory();

    uint8_t *get_physical_addr(uint32_t virtual_addr);

    uint32_t read_word(uint32_t addr);
    uint8_t read_byte(uint32_t addr);
    void read_memory(uint32_t addr, void *buf, uint32_t len);

    void write_word(uint32_t addr, uint32_t word);
    void write_byte(uint32_t addr, uint8_t data);
    void write_memory(uint32_t addr, void *buf, uint32_t len);
};

typedef std::shared_ptr<Memory> SharedMemory;

#endif //MIPS86CORE_MEMORY_H
