#include <stdio.h>
#include "emulator.h"
#include "cpu.h"

void playGame(char *romPath) {
    CpuState cpuState = initializeCpu(romPath);
    dispatchLoop(&cpuState);
}