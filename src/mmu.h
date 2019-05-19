#ifndef MMU_H
#define MMU_H

typedef struct Memory {
    unsigned char rom[0x8000];
    unsigned char graphicsRam[0x2000];
    unsigned char spriteRam[0xa0];
    unsigned char workingRam[0x3e00];
    unsigned char externalRam[0x2000];
    unsigned char zeroPageRam[0x80];
} Memory;

Memory initializeMemory();
void loadGame(Memory* memory, char*);

// unsigned char readByte(Memory*, unsigned short);
// unsigned short readWord(Memory*, unsigned short);

// void writeByte(Memory*, unsigned short, unsigned char);
// void writeWord(Memory*, unsigned short, unsigned short);

#endif