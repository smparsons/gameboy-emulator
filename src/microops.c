#include <stdio.h>
#include "constants.h"
#include "cpu.h"
#include "mmu.h"

unsigned char readByteFromMemory(CpuState* cpuState, unsigned short address) {
    unsigned char byte = readByte(cpuState->memory, address);
    cpuState->clock.totalClockCycles += 4;
    return byte;
}

void storeByteInMemory(CpuState* cpuState, unsigned short address, unsigned char byte) {
    writeByte(cpuState->memory, address, byte);
    cpuState->clock.totalClockCycles += 4;
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

unsigned char readFromRegister(CpuState* cpuState, Register registerToRead) {
    switch (registerToRead) {
        case registerA:
            return cpuState->registers.a;
        case registerB:
            return cpuState->registers.b;
        case registerC:
            return cpuState->registers.c;
        case registerD:
            return cpuState->registers.d;
        case registerE:
            return cpuState->registers.e;
        case registerH:
            return cpuState->registers.h;
        case registerL:
            return cpuState->registers.l;
        case registerF:
            return cpuState->registers.f;
    }
}