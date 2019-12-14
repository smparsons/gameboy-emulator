#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "emulator.h"
#include "gpu.h"

void clearScreen();
void dispatchLoop(CpuState*, GpuState*);

void playGame(char *romPath) {
    CpuState cpuState = initializeCpu(romPath);
    GpuState gpuState = initializeGpu();
    dispatchLoop(&cpuState, &gpuState);
}

void dispatchLoop(CpuState *cpuState, GpuState *gpuState) {
    while (true) {
        int originalClockCycles = cpuState->clock.totalClockCycles;

        executeNextOpcode(cpuState);

        int currentClockCycles = cpuState->clock.totalClockCycles;
        int elapsedClockCycles = currentClockCycles - originalClockCycles;

        stepGpu(gpuState, elapsedClockCycles);
    }   
}