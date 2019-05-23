#include <stdio.h>
#include "cpu.h"
#include "mmu.h"
#include "constants.h"

unsigned char readByteFromMemory(CpuState* cpuState, unsigned short address) {
    unsigned char byte = readByte(cpuState->memory, cpuState->registers.programCounter++);
    cpuState->clock.totalClockCycles += 4;
    return byte;
}

void storeInRegister(CpuState* cpuState, Register registerToLoad, unsigned char value) {
    switch (registerToLoad) {
        case registerA:
            cpuState->registers.a = value;
            break;
        case registerB:
            cpuState->registers.b = value;
            break;
        case registerC:
            cpuState->registers.c = value;
            break;
        case registerD:
            cpuState->registers.d = value;
            break;
        case registerE:
            cpuState->registers.e = value;
            break;
        case registerH:
            cpuState->registers.h = value;
            break;
        case registerL:
            cpuState->registers.l = value;
            break;
        case registerF:
            cpuState->registers.f = value;
            break; 
    }
}