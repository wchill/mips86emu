#ifndef MIPS86CORE_MEMORYMAPPEDDEVICE_H
#define MIPS86CORE_MEMORYMAPPEDDEVICE_H

#include "Memory.h"

class MemoryMappedDevice {
private:
    SharedMemory memory;
public:
    MemoryMappedDevice(SharedMemory memory) : memory(memory) {};

    virtual ~MemoryMappedDevice() {};

    virtual void tick() = 0;
    virtual void reset() = 0;
    virtual std::string device_name() {
        return "MemoryMappedDevice";
    }
};


#endif //MIPS86CORE_MEMORYMAPPEDDEVICE_H
