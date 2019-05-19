#include <stdio.h>
#include "cpu.h"
#include "mmu.h"

void playGame(char *romPath) {
    CpuState cpuState = initializeCpu(romPath);

    printf("registerA=%d\n", cpuState.registers.a);
    printf("registerB=%d\n", cpuState.registers.b);
    printf("registerC=%d\n", cpuState.registers.c);
    printf("registerD=%d\n", cpuState.registers.d);
    printf("registerE=%d\n", cpuState.registers.e);
    printf("registerH=%d\n", cpuState.registers.h);
    printf("registerL=%d\n", cpuState.registers.l);
    printf("registerF=%d\n", cpuState.registers.f);
    printf("programCounter=%d\n", cpuState.registers.programCounter);
    printf("stackPointer=%d\n", cpuState.registers.stackPointer);
    printf("machineCycles=%d\n", cpuState.registers.machineCycles);
    printf("clockCycles=%d\n", cpuState.registers.clockCycles);

    printf("totalMachineCycles=%d\n", cpuState.clock.totalMachineCycles);
    printf("totalClockCycles=%d\n", cpuState.clock.totalClockCycles);

    Memory memory = *cpuState.memory;

    for (int i = 0; i < 0x8000; i++) {
        printf("%d ", memory.rom[i]);
    }
}