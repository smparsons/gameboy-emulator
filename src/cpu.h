#include "mmu.h"

#ifndef CPU_H
#define CPU_H

typedef struct CpuState
{
    struct {
        unsigned char a, b, c, d, e, h, l, f;
        unsigned short programCounter, stackPointer;
    } registers;

    struct {
        int totalClockCycles;
    } clock;

    Memory* memory;
} CpuState;

CpuState initializeCpu(char*);
void dispatchLoop(CpuState*);

#endif