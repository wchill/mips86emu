#include "cop0.h"
#include "memory_old.h"

void cop0_init() {
    mips_coprocessor *cop = malloc(sizeof(*cop));
    cop->registers = malloc(sizeof(*(cop->registers)) * 32);
    cop->control = malloc(sizeof(*(cop->registers)) * 5);
    cop->num_registers = 32;
    cop->num_control = 5;
    coprocessors[0] = cop;
}

void cop0_cleanup() {
    free(coprocessors[0]->registers);
    free(coprocessors[0]->control);
    free(coprocessors[0]);
    coprocessors[0] = NULL;
}
