#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cpu.h"
#include "mmu.h"

void executeOpcode(unsigned char);

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
            .machineCycles=0,
            .clockCycles=0
        },
        .clock={
            .totalMachineCycles=0,
            .totalClockCycles=0
        },
        .memory=&memory
    };

    return cpuState;
}

void dispatchLoop(CpuState *cpuState) {
    while (true) {
        unsigned char nextOpcode = readByte(cpuState->memory, cpuState->registers.programCounter);
        executeOpcode(nextOpcode);

        cpuState->clock.totalClockCycles += cpuState->registers.clockCycles;
        cpuState->clock.totalMachineCycles += cpuState->registers.machineCycles;
        
        cpuState->registers.programCounter++;
        cpuState->registers.programCounter &= 0xFFFF;
    }
}

void executeOpcode(unsigned char nextOpcode) {
    fprintf(stderr, "Opcode %d has not been implemented!\n", nextOpcode);
    exit(EXIT_FAILURE);
}