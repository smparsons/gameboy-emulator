#include <stdio.h>
#include "cpu.h"
#include "mmu.h"

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
