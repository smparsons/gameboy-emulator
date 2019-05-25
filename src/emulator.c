#include <stdio.h>
#include "cpu.h"
#include "emulator.h"

void playGame(char *romPath) {
    CpuState cpuState = initializeCpu(romPath);
    dispatchLoop(&cpuState);
}