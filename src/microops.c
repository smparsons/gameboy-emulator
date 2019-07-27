#include <stdio.h>
#include "constants.h"
#include "cpu.h"
#include "mmu.h"

unsigned char readByteFromMemory(CpuState* cpuState, unsigned short address) {
    unsigned char byte = readByte(cpuState->memory, address);
    cpuState->clock.totalClockCycles += 4;
    return byte;
}

unsigned short readWordFromMemory(CpuState* cpuState, unsigned short address) {
    unsigned short word = readWord(cpuState->memory, address);
    cpuState->clock.totalClockCycles += 8;
    return word;
}

void storeByteInMemory(CpuState* cpuState, unsigned short address, unsigned char byte) {
    writeByte(cpuState->memory, address, byte);
    cpuState->clock.totalClockCycles += 4;
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

unsigned short readFromRegisterPair(CpuState* cpuState, RegisterPair registerPair) {
    unsigned char firstByte = readFromRegister(cpuState, registerPair.first);
    unsigned char secondByte = readFromRegister(cpuState, registerPair.second);
    return (firstByte << 8) | secondByte;
}

void storeInRegisterPair(CpuState* cpuState, RegisterPair registerPair, unsigned short value) {
    storeInRegister(cpuState, registerPair.first, (value >> 8) & 0xFF);
    storeInRegister(cpuState, registerPair.second, value & 0xFF);
}
