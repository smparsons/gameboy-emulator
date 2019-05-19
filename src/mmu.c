#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "mmu.h"

Memory initializeMemory() {
    Memory memory = { 
        .rom={0}, 
        .videoRam={0}, 
        .objectAttributeMemory={0}, 
        .workingRam={0}, 
        .externalRam={0}, 
        .zeroPageRam={0} 
    };

    return memory;
}

void loadGame(Memory* memory, char *romPath) {
    FILE *filePointer = fopen(romPath, "rb");

    if (filePointer == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    fread(memory->rom, sizeof(memory->rom), 1, filePointer);

    fclose(filePointer);
}

unsigned char readByte(Memory* memory, unsigned short address) {
    switch (address & 0xF000) {
        case 0x0000:
        case 0x1000:
        case 0x2000:
        case 0x3000:
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            return memory->rom[address];
        case 0x8000:
        case 0x9000:
            return memory->videoRam[address & 0x1FFF];
        case 0xA000:
        case 0xB000:
            return memory->externalRam[address & 0x1FFF];
        case 0xC000:
        case 0xD000:
            return memory->workingRam[address & 0x1FFF];
        case 0xE000:
            return memory->workingRam[address & 0x1FFF];
        case 0xF000:
            switch (address & 0x0F00) {
                case 0x000: case 0x100: case 0x200: case 0x300:
                case 0x400: case 0x500: case 0x600: case 0x700:
                case 0x800: case 0x900: case 0xA00: case 0xB00:
                case 0xC00: case 0xD00:
                    return memory->workingRam[address & 0x1FFF];
                case 0xE00:
                    return address < 0xFEA0
                        ? memory->objectAttributeMemory[address & 0xFF]
                        : 0;
                case 0xF00:
                    return address >= 0xFF80
                        ? memory->zeroPageRam[address & 0x7F]
                        : 0; 
            }
    }

    return 0;
}

unsigned short readWord(Memory* memory, unsigned short address) {
    unsigned char firstByte = readByte(memory, address);
    unsigned char secondByte = readByte(memory, address + 1);
    return (firstByte << 8) | secondByte;
}

void writeByte(Memory* memory, unsigned short address, unsigned char byte) {
    switch (address & 0xF000) {
        case 0x0000:
        case 0x1000:
        case 0x2000:
        case 0x3000:
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            memory->rom[address] = byte;
            break;
        case 0x8000:
        case 0x9000:
            memory->videoRam[address & 0x1FFF] = byte;
            break;
        case 0xA000:
        case 0xB000:
            memory->externalRam[address & 0x1FFF] = byte;
            break;
        case 0xC000:
        case 0xD000:
            memory->workingRam[address & 0x1FFF] = byte;
            break;
        case 0xE000:
            memory->workingRam[address & 0x1FFF] = byte;
            break;
        case 0xF000:
            switch (address & 0xF000) {
                case 0x000: case 0x100: case 0x200: case 0x300:
                case 0x400: case 0x500: case 0x600: case 0x700:
                case 0x800: case 0x900: case 0xA00: case 0xB00:
                case 0xC00: case 0xD00:
                    memory->workingRam[address & 0x1FFF] = byte;
                case 0xE00:
                    if (address < 0xFEA0) {
                        memory->objectAttributeMemory[address & 0xFF] = byte;
                    }
                case 0xF00:
                    if (address >= 0xFF80) {
                        memory->zeroPageRam[address & 0x7F] = byte;
                    }
            }
    }
}

void writeWord(Memory* memory, unsigned short address, unsigned short word) {
    writeByte(memory, address, word >> 8);
    writeByte(memory, address + 1, word & 0xFF);
}
