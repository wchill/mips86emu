#include <cstdint>
#include <iostream>
#include "MIPS32_Cpu.h"
#include "EmulatorCLI.h"

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char *argv[]) {
    if(argc < 2) {
        cerr << "Usage: ";
        cerr << argv[0];
        cerr << " <MIPS binary>" << endl;
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if(file == NULL) {
        cerr << "Error opening file: ";
        cerr << argv[1] << endl;
        return 1;
    }

    MIPS32_Cpu cpu = MIPS32_Cpu();

    // Load file into memory
    fseek(file,0,SEEK_END);
    off_t file_size = ftell(file);
    fseek(file,0,SEEK_SET);
    uint8_t *buf = new uint8_t[sizeof(uint8_t) * (file_size)];
    fread(buf, 1, static_cast<size_t>(file_size), file);
    cpu.get_cpu_memory()->write_memory(MEMORY_TEXT, buf, static_cast<uint32_t>(file_size));
    delete[] buf;
    fclose(file);

    unsigned long long cycles = 0;
    while(cpu.tick());

    return 0;
}