#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constants.h"
#include "cpu.h"
#include "mmu.h"
#include "microops.h"

void executeNextOpcode(CpuState*);
void loadImmediateValueInRegister(CpuState*, Register);
void loadSourceRegisterInDestinationRegister(CpuState*, Register, Register);
void loadMemoryByteInDestinationRegister(CpuState*, RegisterPair, Register);
void loadSourceRegisterInMemory(CpuState*, Register, RegisterPair);
void loadImmediateValueInMemory(CpuState*, RegisterPair);
unsigned short concatenateTwoRegisters(CpuState*, Register, Register);

const RegisterPair registerHL = { .first = registerH, .second = registerL };

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
        case 0x36:
            loadImmediateValueInMemory(cpuState, registerHL);
            break;
        case 0x40:
            loadSourceRegisterInDestinationRegister(cpuState, registerB, registerB);
            break;
        case 0x41:
            loadSourceRegisterInDestinationRegister(cpuState, registerC, registerB);
            break;
        case 0x42:
            loadSourceRegisterInDestinationRegister(cpuState, registerD, registerB);
            break;
        case 0x43:
            loadSourceRegisterInDestinationRegister(cpuState, registerE, registerB);
            break;
        case 0x44:
            loadSourceRegisterInDestinationRegister(cpuState, registerH, registerB);
            break;
        case 0x45:
            loadSourceRegisterInDestinationRegister(cpuState, registerL, registerB);
            break;   
        case 0x46:
            loadMemoryByteInDestinationRegister(cpuState, registerHL, registerB);
            break;               
        case 0x48:
            loadSourceRegisterInDestinationRegister(cpuState, registerB, registerC);
            break;
        case 0x49:
            loadSourceRegisterInDestinationRegister(cpuState, registerC, registerC);
            break;
        case 0x4A:
            loadSourceRegisterInDestinationRegister(cpuState, registerD, registerC);
            break;
        case 0x4B:
            loadSourceRegisterInDestinationRegister(cpuState, registerE, registerC);
            break;
        case 0x4C:
            loadSourceRegisterInDestinationRegister(cpuState, registerH, registerC);
            break;
        case 0x4D:
            loadSourceRegisterInDestinationRegister(cpuState, registerL, registerC);
            break;
        case 0x4E:
            loadMemoryByteInDestinationRegister(cpuState, registerHL, registerC);
            break;
        case 0x50:
            loadSourceRegisterInDestinationRegister(cpuState, registerB, registerD);
            break;
        case 0x51:
            loadSourceRegisterInDestinationRegister(cpuState, registerC, registerD);
            break;
        case 0x52:
            loadSourceRegisterInDestinationRegister(cpuState, registerD, registerD);
            break;
        case 0x53:
            loadSourceRegisterInDestinationRegister(cpuState, registerE, registerD);
            break;
        case 0x54:
            loadSourceRegisterInDestinationRegister(cpuState, registerH, registerD);
            break;
        case 0x55:
            loadSourceRegisterInDestinationRegister(cpuState, registerL, registerD);
            break;
        case 0x56:
            loadMemoryByteInDestinationRegister(cpuState, registerHL, registerD);
            break;
        case 0x58:
            loadSourceRegisterInDestinationRegister(cpuState, registerB, registerE);
            break;
        case 0x59:
            loadSourceRegisterInDestinationRegister(cpuState, registerC, registerE);
            break;
        case 0x5A:
            loadSourceRegisterInDestinationRegister(cpuState, registerD, registerE);
            break;
        case 0x5B:
            loadSourceRegisterInDestinationRegister(cpuState, registerE, registerE);
            break;
        case 0x5C:
            loadSourceRegisterInDestinationRegister(cpuState, registerH, registerE);
            break;
        case 0x5D:
            loadSourceRegisterInDestinationRegister(cpuState, registerL, registerE);
            break;
        case 0x5E:
            loadMemoryByteInDestinationRegister(cpuState, registerHL, registerE);
            break;
        case 0x60:
            loadSourceRegisterInDestinationRegister(cpuState, registerB, registerH);
            break;
        case 0x61:
            loadSourceRegisterInDestinationRegister(cpuState, registerC, registerH);
            break;
        case 0x62: 
            loadSourceRegisterInDestinationRegister(cpuState, registerD, registerH);
            break;
        case 0x63: 
            loadSourceRegisterInDestinationRegister(cpuState, registerE, registerH);
            break;
        case 0x64: 
            loadSourceRegisterInDestinationRegister(cpuState, registerH, registerH);
            break;
        case 0x65: 
            loadSourceRegisterInDestinationRegister(cpuState, registerL, registerH);
            break;
        case 0x66: 
            loadMemoryByteInDestinationRegister(cpuState, registerHL, registerH);
            break;
        case 0x68: 
            loadSourceRegisterInDestinationRegister(cpuState, registerB, registerL);
            break;
        case 0x69: 
            loadSourceRegisterInDestinationRegister(cpuState, registerC, registerL);
            break;
        case 0x6A: 
            loadSourceRegisterInDestinationRegister(cpuState, registerD, registerL);
            break;
        case 0x6B: 
            loadSourceRegisterInDestinationRegister(cpuState, registerE, registerL);
            break;
        case 0x6C: 
            loadSourceRegisterInDestinationRegister(cpuState, registerH, registerL);
            break;
        case 0x6D: 
            loadSourceRegisterInDestinationRegister(cpuState, registerL, registerL);
            break;
        case 0x6E: 
            loadMemoryByteInDestinationRegister(cpuState, registerHL, registerL);
            break;
        case 0x70: 
            loadSourceRegisterInMemory(cpuState, registerB, registerHL);
            break;
        case 0x71: 
            loadSourceRegisterInMemory(cpuState, registerC, registerHL);
            break;
        case 0x72:
            loadSourceRegisterInMemory(cpuState, registerD, registerHL); 
            break;
        case 0x73: 
            loadSourceRegisterInMemory(cpuState, registerE, registerHL);
            break;
        case 0x74: 
            loadSourceRegisterInMemory(cpuState, registerH, registerHL);
            break;
        case 0x75: 
            loadSourceRegisterInMemory(cpuState, registerL, registerHL);
            break;
        case 0x78:
            loadSourceRegisterInDestinationRegister(cpuState, registerB, registerA);
            break;
        case 0x79:
            loadSourceRegisterInDestinationRegister(cpuState, registerC, registerA);
            break;
        case 0x7A:
            loadSourceRegisterInDestinationRegister(cpuState, registerD, registerA);
            break;
        case 0x7B:
            loadSourceRegisterInDestinationRegister(cpuState, registerE, registerA);
            break;
        case 0x7C:
            loadSourceRegisterInDestinationRegister(cpuState, registerH, registerA);
            break;
        case 0x7D:
            loadSourceRegisterInDestinationRegister(cpuState, registerL, registerA);
            break;
        case 0x7E:
            loadMemoryByteInDestinationRegister(cpuState, registerHL, registerA);
            break;
        case 0x7F:
            loadSourceRegisterInDestinationRegister(cpuState, registerA, registerA);
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

void loadSourceRegisterInDestinationRegister(CpuState *cpuState, Register sourceRegister, Register destinationRegister) {
    unsigned char sourceRegisterValue = readFromRegister(cpuState, sourceRegister);
    storeInRegister(cpuState, destinationRegister, sourceRegisterValue);
}

void loadMemoryByteInDestinationRegister(CpuState *cpuState, RegisterPair addressRegisterPair, Register destinationRegister) {
    unsigned short memoryAddress = concatenateTwoRegisters(cpuState, addressRegisterPair.first, addressRegisterPair.second);
    unsigned char memoryByte = readByteFromMemory(cpuState, memoryAddress);
    storeInRegister(cpuState, destinationRegister, memoryByte);
}

void loadSourceRegisterInMemory(CpuState *cpuState, Register sourceRegister, RegisterPair addressRegisterPair) {
    unsigned char memoryAddress = concatenateTwoRegisters(cpuState, addressRegisterPair.first, addressRegisterPair.second);
    unsigned char memoryValue = readByteFromMemory(cpuState, sourceRegister);
    storeByteInMemory(cpuState, memoryAddress, memoryValue);
}

void loadImmediateValueInMemory(CpuState *cpuState, RegisterPair addressRegisterPair) {
    unsigned char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
    unsigned short memoryAddress = concatenateTwoRegisters(cpuState, addressRegisterPair.first, addressRegisterPair.second);
    storeByteInMemory(cpuState, memoryAddress, immediateValue);
}

unsigned short concatenateTwoRegisters(CpuState *cpuState, Register firstRegister, Register secondRegister) {
    unsigned char firstByte = readFromRegister(cpuState, firstRegister);
    unsigned char secondByte = readFromRegister(cpuState, secondRegister);
    return (firstByte << 8) | secondByte;
}