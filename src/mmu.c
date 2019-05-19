#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "mmu.h"

Memory initializeMemory() {
    Memory memory = { 
        .rom={0}, 
        .graphicsRam={0}, 
        .spriteRam={0}, 
        .workingRam={0}, 
        .externalRam={0}, 
        .zeroPageRam={0} 
    };

    return memory;
}

void loadGame(Memory* memory, char *romPath) {
    FILE *ptr;
    ptr = fopen(romPath, "rb");

    if (ptr == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    fread(memory->rom, sizeof(memory->rom), 1, ptr);

    fclose(ptr);
}