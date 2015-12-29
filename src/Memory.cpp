#include "Memory.h"
#include <iostream>

// For some reason CLion doesn't like fmt::sprintf
#pragma clang diagnostic push
#pragma ide diagnostic ignored "IncompatibleTypes"

Memory::Memory() {
    memory_init(4294967296ull);
    set_endianness(false);
}

Memory::Memory(uint64_t max_memory) {
    memory_init(max_memory);
}

Memory::~Memory() {
    clear_memory();
    delete[] memory_pages;
    memory_pages = NULL;
}

void Memory::set_endianness(bool use_little_endian) {
    little_endian = use_little_endian;
}

uint32_t Memory::swap_endian(uint32_t x) {
    if(little_endian != is_little_endian()) {
        return __bswap_32(x);
    } else {
        return x;
    }
}

void Memory::memory_init(uint64_t max_memory) {
    max_pages = static_cast<unsigned int>(max_memory / PAGE_SIZE);
    allocated_pages = 0;
    memory_pages = new uint8_t*[max_pages]();
}

unsigned int Memory::get_page_num(uint32_t virtual_addr) {
    return virtual_addr >> 12;
}

uint8_t *Memory::get_physical_addr(uint32_t virtual_addr) {
    return memory_pages[get_page_num(virtual_addr)] + (virtual_addr & 0xFFF);
}

void Memory::allocate_page(unsigned int page) {
    if(page >= max_pages) {
        throw std::runtime_error("Page allocation out of bounds");
    }
    if(memory_pages[page] == NULL) {
        //printf("Allocating new page %d\n", page_num);
        memory_pages[page] = new uint8_t[PAGE_SIZE]();
        allocated_pages++;
    }
}

void Memory::clear_memory() {
    allocated_pages = 0;
    for(int i = 0; i < max_pages; i++) {
        delete[] memory_pages[i];
        memory_pages[i] = NULL;
    }
}

uint32_t Memory::read_word(uint32_t addr) {
    unsigned int page_num = get_page_num(addr);
    unsigned int offset = addr & 0xFFF;
    if(page_num >= max_pages || memory_pages[page_num] == NULL) {
        throw std::runtime_error(fmt::sprintf("Invalid read word @ %#08x", addr));
    } else if (addr & ~ADDR_ALIGN_MASK) {
        throw std::runtime_error(fmt::sprintf("Unaligned read word @ %#08x", addr));
    }
    return swap_endian(*(reinterpret_cast<uint32_t*>(get_physical_addr(addr))));
}

uint8_t Memory::read_byte(uint32_t addr) {
    unsigned int page_num = get_page_num(addr);
    if(page_num >= max_pages || memory_pages[page_num] == NULL) {
        throw std::runtime_error(fmt::sprintf("Invalid read byte @ %#08x", addr));
    }
    return *(get_physical_addr(addr));
}

// Check for correct endianness!
void Memory::read_memory(uint32_t addr, void *buf, uint32_t len) {
    for(int i = 0; i < len; i += PAGE_SIZE) {
        uint32_t page_num = get_page_num(addr + i);
        if(page_num >= max_pages || memory_pages[page_num] == NULL) {
            throw std::runtime_error(fmt::sprintf("Invalid read memory @ %#08x", addr));
        }
        if(len - i < PAGE_SIZE) {
            memcpy(static_cast<uint8_t*>(buf) + i, memory_pages[page_num], len - i);
        } else {
            memcpy(static_cast<uint8_t*>(buf) + i, memory_pages[page_num], 4096);
        }
    }
}

void Memory::write_word(uint32_t addr, uint32_t word) {
    unsigned int page_num = get_page_num(addr);
    unsigned int offset = addr & 0xFFF;
    if(page_num >= max_pages || memory_pages[page_num] == NULL) {
        throw std::runtime_error(fmt::sprintf("Invalid write word @ %#08x", addr));
    } else if (addr & ~ADDR_ALIGN_MASK) {
        throw std::runtime_error(fmt::sprintf("Unaligned write word @ %#08x", addr));
    }
    allocate_page(page_num);
    *(reinterpret_cast<uint32_t*>(memory_pages[page_num] + offset)) = swap_endian(word);
}

void Memory::write_byte(uint32_t addr, uint8_t word) {
    unsigned int page_num = get_page_num(addr);
    unsigned int offset = addr & 0xFFF;
    if(page_num >= max_pages || memory_pages[page_num] == NULL) {
        throw std::runtime_error(fmt::sprintf("Invalid write byte @ %#08x", addr));
    }
    allocate_page(page_num);
    *(memory_pages[page_num] + offset) = word;
}

void Memory::write_memory(uint32_t addr, void *buf, uint32_t len) {
    for(int i = 0; i < len; i += PAGE_SIZE) {
        uint32_t page_num = get_page_num(addr + i);
        allocate_page(page_num);
        if(len - i < PAGE_SIZE) {
            memcpy(memory_pages[page_num], static_cast<uint8_t*>(buf) + i, len - i);
        } else {
            memcpy(memory_pages[page_num], static_cast<uint8_t*>(buf) + i, PAGE_SIZE);
        }
    }
}
#pragma clang diagnostic pop