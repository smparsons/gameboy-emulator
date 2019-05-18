#include <stdbool.h>

#ifndef MMU_H
#define MMU_H

typedef struct Memory {
    bool inBios;
    unsigned char bios[0x100];
    unsigned char rom[0x8000];
    unsigned char graphicsRam[0x2000];
    unsigned char spriteRam[0xa0];
    unsigned char workingRam[0x3e00];
    unsigned char externalRam[0x2000];
    unsigned char zeroPageRam[0x80];
} Memory;

unsigned char readByte(Memory* memory, unsigned short address);
unsigned short readWord(Memory* memory, unsigned short address);

void writeByte(Memory* memory, unsigned short address, unsigned char byte);
void writeWord(Memory* memory, unsigned short address, unsigned short word);

#endif