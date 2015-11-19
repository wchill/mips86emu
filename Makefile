EMULATOR = mips86emu
OUTPUT_DIR = bin
OUTPUT = $(OUTPUT_DIR)/$(EMULATOR)

CC = gcc
WARNINGS = -Wall
CC_OPTS = -c -g -O0 -Wfatal-errors -Werror $(WARNINGS)

all: $(OUTPUT)

clean:
	rm -rf bin *.o

$(OUTPUT): emulator.o memory.o inst_r.o inst_i.o coprocessor.o cop0.o cop1.o
	mkdir -p bin
	$(CC) -o $@ $^

emulator.o: emulator.c mips.h register.h instructions.h
	$(CC) $< $(CC_OPTS)

memory.o: memory.c memory.h
	$(CC) $< $(CC_OPTS)

inst_r.o: inst_r.c inst_r.h
	$(CC) $< $(CC_OPTS)

inst_i.o: inst_i.c inst_i.h coprocessor.o
	$(CC) $< $(CC_OPTS)

cop0.o: cop0.c cop1.h coprocessor.o
	$(CC) $< $(CC_OPTS)

cop1.o: cop1.c cop1.h coprocessor.o
	$(CC) $< $(CC_OPTS)

coprocessor.o: coprocessor.c coprocessor.h
	$(CC) $< $(CC_OPTS)
