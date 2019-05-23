#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cpu.h"
#include "mmu.h"
#include "microops.h"
#include "constants.h"

void executeNextOpcode(CpuState*);
void loadImmediateValueInRegister(CpuState*, Register);

CpuState initializeCpu(char *romPath) {
    Memory memory = initializeMemory();
    loadGame(&memory, romPath);

    CpuState cpuState = {
        .registers={
            .a=0,
            .b=0,
            .c=0,
            .d=0,
            .e=0,
            .h=0,
            .l=0,
            .f=0,
            .programCounter=0,
            .stackPointer=0,
        },
        .clock={
            .totalClockCycles=0
        },
        .memory=&memory
    };

    return cpuState;
}

void dispatchLoop(CpuState *cpuState) {
    while (true) {
        executeNextOpcode(cpuState);
    }
}

void executeNextOpcode(CpuState *cpuState) {
    unsigned char nextOpcode = readByteFromMemory(cpuState, cpuState->registers.programCounter++);

    switch (nextOpcode) {
        case 0x06:
            loadImmediateValueInRegister(cpuState, registerB);
            break;
        case 0x0E:
            loadImmediateValueInRegister(cpuState, registerC);
            break;
        case 0x16:
            loadImmediateValueInRegister(cpuState, registerD);
            break;
        case 0x1E:
            loadImmediateValueInRegister(cpuState, registerE);
            break;
        case 0x26:
            loadImmediateValueInRegister(cpuState, registerH);
            break;
        case 0x2E:
            loadImmediateValueInRegister(cpuState, registerL);
            break;
        default:
            fprintf(stderr, "Opcode %d has not been implemented!\n", nextOpcode);
            exit(EXIT_FAILURE);
    }

    cpuState->registers.programCounter &= 0xFFFF;
}

void loadImmediateValueInRegister(CpuState *cpuState, Register registerToLoad) {
    unsigned char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
    storeInRegister(cpuState, registerToLoad, immediateValue);
}