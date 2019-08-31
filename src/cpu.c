#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constants.h"
#include "cpu.h"
#include "mmu.h"
#include "microops.h"

void executeNextOpcode(CpuState*);
void loadImmediateByteInRegister(CpuState*, Register);
void loadImmediateByteInMemory(CpuState*, short);
void loadImmediateWordInRegisterPair(CpuState*, RegisterPair);
void loadSourceRegisterInDestinationRegister(CpuState*, Register, Register);
void loadMemoryByteInDestinationRegister(CpuState*, short, Register);
void loadSourceRegisterInMemory(CpuState*, Register, short);
void handleIllegalOpcode(unsigned char);

const RegisterPair registerBC = { .first = registerB, .second = registerC };
const RegisterPair registerDE = { .first = registerD, .second = registerE };
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
        case 0x00:
            break;
        case 0x01:
            loadImmediateWordInRegisterPair(cpuState, registerBC);
            break;
        case 0x02: {
            unsigned short address = readFromRegisterPair(cpuState, registerBC);
            loadSourceRegisterInMemory(cpuState, registerA, address);
            break;
        }
        case 0x06:
            loadImmediateByteInRegister(cpuState, registerB);
            break;
        case 0x08: {
            unsigned short address = readWordFromMemory(cpuState, cpuState->registers.programCounter++);
            storeWordInMemory(cpuState, address, cpuState->registers.stackPointer);
            break;
        }
        case 0x0A: {
            unsigned short address = readFromRegisterPair(cpuState, registerBC);
            loadMemoryByteInDestinationRegister(cpuState, address, registerA);
            break;
        }
        case 0x0E:
            loadImmediateByteInRegister(cpuState, registerC);
            break;
        case 0x11:
            loadImmediateWordInRegisterPair(cpuState, registerDE);
            break;
        case 0x12: {
            unsigned short address = readFromRegisterPair(cpuState, registerDE);
            loadSourceRegisterInMemory(cpuState, registerA, address);
            break;
        }
        case 0x16:
            loadImmediateByteInRegister(cpuState, registerD);
            break;
        case 0x1A: {
            unsigned short address = readFromRegisterPair(cpuState, registerDE);
            loadMemoryByteInDestinationRegister(cpuState, address, registerA);
            break;
        }
        case 0x1E:
            loadImmediateByteInRegister(cpuState, registerE);
            break;
        case 0x21:
            loadImmediateWordInRegisterPair(cpuState, registerHL);
            break;
        case 0x22: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadSourceRegisterInMemory(cpuState, registerA, address);
            storeInRegisterPair(cpuState, registerHL, address++);
            break;
        }
        case 0x26:
            loadImmediateByteInRegister(cpuState, registerH);
            break;
        case 0x2A: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadMemoryByteInDestinationRegister(cpuState, address, registerA);
            storeInRegisterPair(cpuState, registerHL, address++);
            break;
        }
        case 0x2E:
            loadImmediateByteInRegister(cpuState, registerL);
            break;
        case 0x31: {
            unsigned short immediateValue = readWordFromMemory(cpuState, cpuState->registers.programCounter++);
            cpuState->registers.stackPointer = immediateValue;
            break;
        }
        case 0x32: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadSourceRegisterInMemory(cpuState, registerA, address);
            storeInRegisterPair(cpuState, registerHL, address--);
            break;
        }
        case 0x36: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadImmediateByteInMemory(cpuState, address);
            break;
        }
        case 0x3A: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadMemoryByteInDestinationRegister(cpuState, address, registerA);
            storeInRegisterPair(cpuState, registerHL, address--);
            break;
        }
        case 0x3E:
            loadImmediateByteInRegister(cpuState, registerA);
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
        case 0x46: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadMemoryByteInDestinationRegister(cpuState, address, registerB);
            break;  
        }
        case 0x47:
            loadSourceRegisterInDestinationRegister(cpuState, registerA, registerB);
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
        case 0x4E: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadMemoryByteInDestinationRegister(cpuState, address, registerC);
            break;
        }
        case 0x4F:
            loadSourceRegisterInDestinationRegister(cpuState, registerA, registerC);
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
        case 0x56: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadMemoryByteInDestinationRegister(cpuState, address, registerD);
            break;
        }
        case 0x57:
            loadSourceRegisterInDestinationRegister(cpuState, registerA, registerD);
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
        case 0x5E: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadMemoryByteInDestinationRegister(cpuState, address, registerE);
            break;
        }
        case 0x5F:
            loadSourceRegisterInDestinationRegister(cpuState, registerA, registerE);
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
        case 0x66: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadMemoryByteInDestinationRegister(cpuState, address, registerH);
            break;
        }
        case 0x67:
            loadSourceRegisterInDestinationRegister(cpuState, registerA, registerH);
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
        case 0x6E: { 
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadMemoryByteInDestinationRegister(cpuState, address, registerL);
            break;
        }
        case 0x6F:
            loadSourceRegisterInDestinationRegister(cpuState, registerA, registerL);
            break;
        case 0x70: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL); 
            loadSourceRegisterInMemory(cpuState, registerB, address);
            break;
        }
        case 0x71: { 
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadSourceRegisterInMemory(cpuState, registerC, address);
            break;
        }
        case 0x72: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadSourceRegisterInMemory(cpuState, registerD, address); 
            break;
        }
        case 0x73: { 
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadSourceRegisterInMemory(cpuState, registerE, address);
            break;
        }
        case 0x74: { 
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadSourceRegisterInMemory(cpuState, registerH, address);
            break;
        }
        case 0x75: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadSourceRegisterInMemory(cpuState, registerL, address);
            break;
        }
        case 0x77: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadSourceRegisterInMemory(cpuState, registerA, address);
            break;
        }
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
        case 0x7E: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadMemoryByteInDestinationRegister(cpuState, address, registerA);
            break;
        }
        case 0x7F:
            loadSourceRegisterInDestinationRegister(cpuState, registerA, registerA);
            break;
        case 0xD3:
            handleIllegalOpcode(nextOpcode);
            break;
        case 0xDB:
            handleIllegalOpcode(nextOpcode);
            break;
        case 0xDD:
            handleIllegalOpcode(nextOpcode);
            break;
        case 0xE0: {
            unsigned char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
            unsigned char registerValue = readFromRegister(cpuState, registerA);
            storeByteInMemory(cpuState, 0xFF00 + immediateValue, registerValue);
            break;
        }
        case 0xE2: {
            unsigned char registerAValue = readFromRegister(cpuState, registerA);
            unsigned char registerCValue = readFromRegister(cpuState, registerC);
            storeByteInMemory(cpuState, 0xFF00 + registerCValue, registerAValue);
            break;
        }
        case 0xE3:
            handleIllegalOpcode(nextOpcode);
            break;
        case 0xE4:
            handleIllegalOpcode(nextOpcode);
            break;
        case 0xEA: {
            unsigned short immediateValueAddress = cpuState->registers.programCounter + 1;
            unsigned short address = readWordFromMemory(cpuState, immediateValueAddress);
            loadSourceRegisterInMemory(cpuState, registerA, address);
            cpuState->registers.programCounter += 2;
            break;
        }
        case 0xEB:
            handleIllegalOpcode(nextOpcode);
            break;  
        case 0xEC:
            handleIllegalOpcode(nextOpcode);
            break;
        case 0xED:
            handleIllegalOpcode(nextOpcode);
            break;
        case 0xF0: {
            unsigned char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
            loadMemoryByteInDestinationRegister(cpuState, 0xFF00 + immediateValue, registerA);
            break;
        }
        case 0xF2: {
            unsigned char registerValue = readFromRegister(cpuState, registerC);
            loadMemoryByteInDestinationRegister(cpuState, 0xFF00 + registerValue, registerA);
            break;
        }
        case 0xF4:
            handleIllegalOpcode(nextOpcode);
            break;
        case 0xF8: {
            unsigned char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
            unsigned short valueToStore = cpuState->registers.stackPointer + immediateValue;        
            storeInRegisterPair(cpuState, registerHL, valueToStore);
            runExtraMachineCycle(cpuState);
            break;
        }
        case 0xF9: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            cpuState->registers.stackPointer = address;
            runExtraMachineCycle(cpuState);
            break;
        }
        case 0xFA: {
            unsigned short immediateValueAddress = cpuState->registers.programCounter + 1;
            unsigned short address = readWordFromMemory(cpuState, immediateValueAddress);
            loadMemoryByteInDestinationRegister(cpuState, address, registerA);
            cpuState->registers.programCounter += 2;
            break;
        }
        case 0xFC:
            handleIllegalOpcode(nextOpcode);
            break;
        case 0xFD:
            handleIllegalOpcode(nextOpcode);
            break;
        default:
            fprintf(stderr, "Opcode %d has not been implemented!\n", nextOpcode);
            exit(EXIT_FAILURE);
    }

    cpuState->registers.programCounter &= 0xFFFF;
}

void loadImmediateByteInRegister(CpuState *cpuState, Register registerToLoad) {
    unsigned char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
    storeInRegister(cpuState, registerToLoad, immediateValue);
}

void loadImmediateByteInMemory(CpuState *cpuState, short destinationAddress) {
    unsigned char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
    storeByteInMemory(cpuState, destinationAddress, immediateValue);
}

void loadImmediateWordInRegisterPair(CpuState *cpuState, RegisterPair registerPairToLoad) {
    unsigned short immediateValue = readWordFromMemory(cpuState, cpuState->registers.programCounter++);
    storeInRegisterPair(cpuState, registerPairToLoad, immediateValue);
}

void loadSourceRegisterInDestinationRegister(CpuState *cpuState, Register sourceRegister, Register destinationRegister) {
    unsigned char sourceRegisterValue = readFromRegister(cpuState, sourceRegister);
    storeInRegister(cpuState, destinationRegister, sourceRegisterValue);
}

void loadMemoryByteInDestinationRegister(CpuState *cpuState, short sourceAddress, Register destinationRegister) {
    unsigned char memoryByte = readByteFromMemory(cpuState, sourceAddress);
    storeInRegister(cpuState, destinationRegister, memoryByte);
}

void loadSourceRegisterInMemory(CpuState *cpuState, Register sourceRegister, short destinationAddress) {
    unsigned char memoryValue = readFromRegister(cpuState, sourceRegister);
    storeByteInMemory(cpuState, destinationAddress, memoryValue);
}

void handleIllegalOpcode(unsigned char opcode) {
    fprintf(stderr, "Encountered illegal opcode %d!\n", opcode);
    exit(EXIT_FAILURE);
}