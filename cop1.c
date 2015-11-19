#include "cop1.h"
#include "memory.h"

void cop1_init() {
    mips_coprocessor *cop = malloc(sizeof(*cop));
    cop->registers = malloc(sizeof(*(cop->registers)) * 32);
    cop->control = malloc(sizeof(*(cop->registers)) * 5);
    cop->num_registers = 32;
    cop->num_control = 5;
    coprocessors[1] = cop;
}

void cop1_cleanup() {
    free(coprocessors[1]->registers);
    free(coprocessors[1]->control);
    free(coprocessors[1]);
    coprocessors[1] = NULL;
}
