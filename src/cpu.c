#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constants.h"
#include "cpu.h"
#include "mmu.h"
#include "microops.h"

void loadImmediateByteInRegister(CpuState*, Register);
void loadImmediateByteInMemory(CpuState*, unsigned short);
void loadImmediateWordInRegisterPair(CpuState*, RegisterPair);
void loadSourceRegisterInDestinationRegister(CpuState*, Register, Register);
void loadMemoryByteInDestinationRegister(CpuState*, unsigned short, Register);
void loadSourceRegisterInMemory(CpuState*, Register, unsigned short);
void addToRegister(CpuState*, Register, unsigned char);
void addToRegisterWithCarry(CpuState*, Register, unsigned char);
void addToRegisterPair(CpuState*, RegisterPair, unsigned short);
void compareRegister(CpuState*, Register, unsigned char);
void logicalAndWithRegister(CpuState*, Register, unsigned char);
void logicalOrWithRegister(CpuState*, Register, unsigned char);
void logicalExclusiveOrWithRegister(CpuState*, Register, unsigned char);
void incrementRegister(CpuState*, Register);
void incrementRegisterPair(CpuState*, RegisterPair);
void incrementMemoryByte(CpuState*, RegisterPair);
void decrementRegister(CpuState*, Register);
void decrementRegisterPair(CpuState*, RegisterPair);
void decrementMemoryByte(CpuState*, RegisterPair);
bool getZFlag(CpuState*);
bool getCFlag(CpuState*);
void pushRegisterPair(CpuState*, RegisterPair);
void popRegisterPair(CpuState*, RegisterPair);
void stackReturn(CpuState*);
void restart(CpuState*, unsigned short);
void handleIllegalOpcode(unsigned char);

const RegisterPair registerAF = { .first = registerA, .second = registerF };
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

void executeNextOpcode(CpuState *cpuState) {
    unsigned char nextOpcode = readByteFromMemory(cpuState, cpuState->registers.programCounter++);

    printf("CPU: Executing opcode 0x%X...\n", nextOpcode);

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
        case 0x03:
            incrementRegisterPair(cpuState, registerBC);
            break;
        case 0x04:
            incrementRegister(cpuState, registerB);
            break;
        case 0x05:
            decrementRegister(cpuState, registerB);
            break;
        case 0x06:
            loadImmediateByteInRegister(cpuState, registerB);
            break;
        case 0x08: {
            unsigned short address = readWordFromMemory(cpuState, cpuState->registers.programCounter);
            storeWordInMemory(cpuState, address, cpuState->registers.stackPointer);
            cpuState->registers.programCounter += 2;
            break;
        }
        case 0x09:
            addToRegisterPair(cpuState, registerHL, readFromRegisterPair(cpuState, registerBC));
            break;
        case 0x0A: {
            unsigned short address = readFromRegisterPair(cpuState, registerBC);
            loadMemoryByteInDestinationRegister(cpuState, address, registerA);
            break;
        }
        case 0x0B:
            decrementRegisterPair(cpuState, registerBC);
            break;
        case 0x0C:
            incrementRegister(cpuState, registerC);
            break;
        case 0x0D:
            decrementRegister(cpuState, registerC);
            break;
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
        case 0x13:
            incrementRegisterPair(cpuState, registerDE);
            break;
        case 0x14:
            incrementRegister(cpuState, registerD);
            break;
        case 0x15:
            decrementRegister(cpuState, registerD);
            break;
        case 0x16:
            loadImmediateByteInRegister(cpuState, registerD);
            break;
        case 0x18: {
            char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
            cpuState->registers.programCounter += immediateValue;
            break;
        }
        case 0x19:
            addToRegisterPair(cpuState, registerHL, readFromRegisterPair(cpuState, registerDE));
            break;
        case 0x1A: {
            unsigned short address = readFromRegisterPair(cpuState, registerDE);
            loadMemoryByteInDestinationRegister(cpuState, address, registerA);
            break;
        }
        case 0x1B:
            decrementRegisterPair(cpuState, registerDE);
            break;
        case 0x1C:
            incrementRegister(cpuState, registerE);
            break;
        case 0x1D:
            decrementRegister(cpuState, registerE);
            break;
        case 0x1E:
            loadImmediateByteInRegister(cpuState, registerE);
            break;
        case 0x20: {
            char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
            bool zFlag = getZFlag(cpuState);
            if (!zFlag) cpuState->registers.programCounter += immediateValue;
            break;
        }
        case 0x21:
            loadImmediateWordInRegisterPair(cpuState, registerHL);
            break;
        case 0x22: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadSourceRegisterInMemory(cpuState, registerA, address);
            storeInRegisterPair(cpuState, registerHL, address++);
            break;
        }
        case 0x23:
            incrementRegisterPair(cpuState, registerHL);
            break;
        case 0x24:
            incrementRegister(cpuState, registerH);
            break;
        case 0x25:
            decrementRegister(cpuState, registerH);
            break;
        case 0x26:
            loadImmediateByteInRegister(cpuState, registerH);
            break;
        case 0x28: {
            char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
            bool zFlag = getZFlag(cpuState);
            if (zFlag) cpuState->registers.programCounter += immediateValue;
            break;
        }
        case 0x29:
            addToRegisterPair(cpuState, registerHL, readFromRegisterPair(cpuState, registerHL));
            break;
        case 0x2A: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadMemoryByteInDestinationRegister(cpuState, address, registerA);
            storeInRegisterPair(cpuState, registerHL, address++);
            break;
        }
        case 0x2B:
            decrementRegisterPair(cpuState, registerHL);
            break;
        case 0x2C:
            incrementRegister(cpuState, registerL);
            break;        
        case 0x2D:
            decrementRegister(cpuState, registerL);
            break;
        case 0x2E:
            loadImmediateByteInRegister(cpuState, registerL);
            break;
        case 0x30: {
            char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
            bool cFlag = getCFlag(cpuState);
            if (!cFlag) cpuState->registers.programCounter += immediateValue;
            break;
        }
        case 0x31: {
            unsigned short immediateValue = readWordFromMemory(cpuState, cpuState->registers.programCounter);
            cpuState->registers.stackPointer = immediateValue;
            cpuState->registers.programCounter += 2;
            break;
        }
        case 0x32: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadSourceRegisterInMemory(cpuState, registerA, address);
            storeInRegisterPair(cpuState, registerHL, address--);
            break;
        }
        case 0x33:
            cpuState->registers.stackPointer++;
            break;
        case 0x34:
            incrementMemoryByte(cpuState, registerHL);
            break;
        case 0x35:
            decrementMemoryByte(cpuState, registerHL);
            break;
        case 0x36: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadImmediateByteInMemory(cpuState, address);
            break;
        }
        case 0x38: {
            char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
            bool cFlag = getCFlag(cpuState);
            if (cFlag) cpuState->registers.programCounter += immediateValue;
            break;
        }
        case 0x39:
            addToRegisterPair(cpuState, registerHL, cpuState->registers.stackPointer);
            break;
        case 0x3A: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            loadMemoryByteInDestinationRegister(cpuState, address, registerA);
            storeInRegisterPair(cpuState, registerHL, address--);
            break;
        }
        case 0x3B:
            cpuState->registers.stackPointer--;
            break;
        case 0x3C:
            incrementRegister(cpuState, registerA);
            break;
        case 0x3D:
            decrementRegister(cpuState, registerA);
            break;
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
        case 0x80:
            addToRegister(cpuState, registerA, readFromRegister(cpuState, registerB));
            break;
        case 0x81:
            addToRegister(cpuState, registerA, readFromRegister(cpuState, registerC));
            break;
        case 0x82:
            addToRegister(cpuState, registerA, readFromRegister(cpuState, registerD));
            break;
        case 0x83:
            addToRegister(cpuState, registerA, readFromRegister(cpuState, registerE));
            break;
        case 0x84:
            addToRegister(cpuState, registerA, readFromRegister(cpuState, registerH));
            break;
        case 0x85:
            addToRegister(cpuState, registerA, readFromRegister(cpuState, registerL));
            break;
        case 0x86: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            unsigned char value = readByteFromMemory(cpuState, address);
            addToRegister(cpuState, registerA, value);
            break;
        }
        case 0x87:
            addToRegister(cpuState, registerA, readFromRegister(cpuState, registerA));
            break;
        case 0x88:
            addToRegisterWithCarry(cpuState, registerA, readFromRegister(cpuState, registerB));
            break;
        case 0x89:
            addToRegisterWithCarry(cpuState, registerA, readFromRegister(cpuState, registerC));
            break;
        case 0x8A:
            addToRegisterWithCarry(cpuState, registerA, readFromRegister(cpuState, registerD));
            break;
        case 0x8B:
            addToRegisterWithCarry(cpuState, registerA, readFromRegister(cpuState, registerE));
            break;
        case 0x8C:
            addToRegisterWithCarry(cpuState, registerA, readFromRegister(cpuState, registerH));
            break;
        case 0x8D:
            addToRegisterWithCarry(cpuState, registerA, readFromRegister(cpuState, registerL));
            break;
        case 0x8E: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            unsigned char value = readByteFromMemory(cpuState, address);
            addToRegisterWithCarry(cpuState, registerA, value);
            break;
        }
        case 0x8F:
            addToRegisterWithCarry(cpuState, registerA, readFromRegister(cpuState, registerA));
            break;
        case 0xA0:
            logicalAndWithRegister(cpuState, registerA, readFromRegister(cpuState, registerB));
            break;
        case 0xA1:
            logicalAndWithRegister(cpuState, registerA, readFromRegister(cpuState, registerC));
            break;
        case 0xA2:
            logicalAndWithRegister(cpuState, registerA, readFromRegister(cpuState, registerD));
            break;
        case 0xA3:
            logicalAndWithRegister(cpuState, registerA, readFromRegister(cpuState, registerE));
            break;
        case 0xA4:
            logicalAndWithRegister(cpuState, registerA, readFromRegister(cpuState, registerH));
            break;
        case 0xA5:
            logicalAndWithRegister(cpuState, registerA, readFromRegister(cpuState, registerL));
            break;
        case 0xA6: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            unsigned char value = readByteFromMemory(cpuState, address);
            logicalAndWithRegister(cpuState, registerA, value);
            break;
        }
        case 0xA7:
            logicalAndWithRegister(cpuState, registerA, readFromRegister(cpuState, registerA));
            break;
        case 0xA8:
            logicalExclusiveOrWithRegister(cpuState, registerA, readFromRegister(cpuState, registerB));
            break;
        case 0xA9:
            logicalExclusiveOrWithRegister(cpuState, registerA, readFromRegister(cpuState, registerC));
            break;
        case 0xAA:
            logicalExclusiveOrWithRegister(cpuState, registerA, readFromRegister(cpuState, registerD));
            break;
        case 0xAB:
            logicalExclusiveOrWithRegister(cpuState, registerA, readFromRegister(cpuState, registerE));
            break;
        case 0xAC:
            logicalExclusiveOrWithRegister(cpuState, registerA, readFromRegister(cpuState, registerH));
            break;
        case 0xAD:
            logicalExclusiveOrWithRegister(cpuState, registerA, readFromRegister(cpuState, registerL));
            break;
        case 0xAE: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            unsigned char value = readByteFromMemory(cpuState, address);
            logicalExclusiveOrWithRegister(cpuState, registerA, value);
            break;
        }
        case 0xAF:
            logicalExclusiveOrWithRegister(cpuState, registerA, readFromRegister(cpuState, registerA));
            break;
        case 0xB0:
            logicalOrWithRegister(cpuState, registerA, readFromRegister(cpuState, registerB));
            break;
        case 0xB1:
            logicalOrWithRegister(cpuState, registerA, readFromRegister(cpuState, registerC));
            break;
        case 0xB2:
            logicalOrWithRegister(cpuState, registerA, readFromRegister(cpuState, registerD));
            break;
        case 0xB3:
            logicalOrWithRegister(cpuState, registerA, readFromRegister(cpuState, registerE));
            break;
        case 0xB4:
            logicalOrWithRegister(cpuState, registerA, readFromRegister(cpuState, registerH));
            break;
        case 0xB5:
            logicalOrWithRegister(cpuState, registerA, readFromRegister(cpuState, registerL));
            break;
        case 0xB6: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            unsigned char value = readByteFromMemory(cpuState, address);
            logicalOrWithRegister(cpuState, registerA, value);
            break;
        }
        case 0xB7:
            logicalOrWithRegister(cpuState, registerA, readFromRegister(cpuState, registerA));
            break;
        case 0xB8:
            compareRegister(cpuState, registerA, readFromRegister(cpuState, registerB));
            break;
        case 0xB9:
            compareRegister(cpuState, registerA, readFromRegister(cpuState, registerC));
            break;
        case 0xBA:
            compareRegister(cpuState, registerA, readFromRegister(cpuState, registerD));
            break;
        case 0xBB:
            compareRegister(cpuState, registerA, readFromRegister(cpuState, registerE));
            break;
        case 0xBC:
            compareRegister(cpuState, registerA, readFromRegister(cpuState, registerH));
            break;
        case 0xBD:
            compareRegister(cpuState, registerA, readFromRegister(cpuState, registerL));
            break;
        case 0xBE: {
            unsigned short address = readFromRegisterPair(cpuState, registerHL);
            unsigned char value = readByteFromMemory(cpuState, address);
            compareRegister(cpuState, registerA, value);
            break;
        }
        case 0xBF:
            compareRegister(cpuState, registerA, readFromRegister(cpuState, registerA));
            break;
        case 0xC1:
            popRegisterPair(cpuState, registerBC);
            break;
        case 0xC3:
            cpuState->registers.programCounter = readWordFromMemory(cpuState, cpuState->registers.programCounter);
            break;
        case 0xC5:
            pushRegisterPair(cpuState, registerBC);
            break;
        case 0xC6: {
            unsigned char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
            addToRegister(cpuState, registerA, immediateValue);
            break;
        }
        case 0xC7:
            restart(cpuState, 0x00);
            break;
        case 0xC9:
            stackReturn(cpuState);
            break;
        case 0xCE: {
            unsigned char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
            addToRegisterWithCarry(cpuState, registerA, immediateValue);
            break;
        }
        case 0xCF:
            restart(cpuState, 0x08);
            break;
        case 0xD1:
            popRegisterPair(cpuState, registerDE);
            break;
        case 0xD3:
            handleIllegalOpcode(nextOpcode);
            break;
        case 0xD5:
            pushRegisterPair(cpuState, registerDE);
            break;
        case 0xD7:
            restart(cpuState, 0x10);
            break;
        case 0xDB:
            handleIllegalOpcode(nextOpcode);
            break;
        case 0xDD:
            handleIllegalOpcode(nextOpcode);
            break;
        case 0xDF:
            restart(cpuState, 0x18);
            break;
        case 0xE0: {
            unsigned char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
            unsigned char registerValue = readFromRegister(cpuState, registerA);
            storeByteInMemory(cpuState, 0xFF00 + immediateValue, registerValue);
            break;
        }
        case 0xE1:
            popRegisterPair(cpuState, registerHL);
            break;
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
        case 0xE5:
            pushRegisterPair(cpuState, registerHL);
            break;
        case 0xE6: {
            unsigned char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
            logicalAndWithRegister(cpuState, registerA, immediateValue);
            break;
        }
        case 0xE7:
            restart(cpuState, 0x20);
            break;
        case 0xE8: {
            char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
            cpuState->registers.stackPointer += immediateValue;
            break;
        }
        case 0xEA: {
            unsigned short address = readWordFromMemory(cpuState, cpuState->registers.programCounter);
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
        case 0xEE: {
            unsigned char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
            logicalExclusiveOrWithRegister(cpuState, registerA, immediateValue);
            break;
        }
        case 0xEF:
            restart(cpuState, 0x28);
            break;
        case 0xF0: {
            unsigned char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
            loadMemoryByteInDestinationRegister(cpuState, 0xFF00 + immediateValue, registerA);
            break;
        }
        case 0xF1:
            popRegisterPair(cpuState, registerAF);
            break;
        case 0xF2: {
            unsigned char registerValue = readFromRegister(cpuState, registerC);
            loadMemoryByteInDestinationRegister(cpuState, 0xFF00 + registerValue, registerA);
            break;
        }
        case 0xF4:
            handleIllegalOpcode(nextOpcode);
            break;
        case 0xF5:
            pushRegisterPair(cpuState, registerAF);
            break;
        case 0xF6: {
            unsigned char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
            logicalOrWithRegister(cpuState, registerA, immediateValue);
            break;
        }
        case 0xF7:
            restart(cpuState, 0x30);
            break;
        case 0xF8: {
            char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
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
            unsigned short address = readWordFromMemory(cpuState, cpuState->registers.programCounter);
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
        case 0xFE:
            compareRegister(cpuState, registerA, readByteFromMemory(cpuState, cpuState->registers.programCounter++));
            break;
        case 0xFF:
            restart(cpuState, 0x38);
            break;
        default:
            fprintf(stderr, "Opcode 0x%X has not been implemented!\n", nextOpcode);
            exit(EXIT_FAILURE);
    }

    cpuState->registers.programCounter &= 0xFFFF;
}

void loadImmediateByteInRegister(CpuState *cpuState, Register registerToLoad) {
    unsigned char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
    storeInRegister(cpuState, registerToLoad, immediateValue);
}

void loadImmediateByteInMemory(CpuState *cpuState, unsigned short destinationAddress) {
    unsigned char immediateValue = readByteFromMemory(cpuState, cpuState->registers.programCounter++);
    storeByteInMemory(cpuState, destinationAddress, immediateValue);
}

void loadImmediateWordInRegisterPair(CpuState *cpuState, RegisterPair registerPairToLoad) {
    unsigned short immediateValue = readWordFromMemory(cpuState, cpuState->registers.programCounter);
    storeInRegisterPair(cpuState, registerPairToLoad, immediateValue);
    cpuState->registers.programCounter += 2;
}

void loadSourceRegisterInDestinationRegister(CpuState *cpuState, Register sourceRegister, Register destinationRegister) {
    unsigned char sourceRegisterValue = readFromRegister(cpuState, sourceRegister);
    storeInRegister(cpuState, destinationRegister, sourceRegisterValue);
}

void loadMemoryByteInDestinationRegister(CpuState *cpuState, unsigned short sourceAddress, Register destinationRegister) {
    unsigned char memoryByte = readByteFromMemory(cpuState, sourceAddress);
    storeInRegister(cpuState, destinationRegister, memoryByte);
}

void loadSourceRegisterInMemory(CpuState *cpuState, Register sourceRegister, unsigned short destinationAddress) {
    unsigned char memoryValue = readFromRegister(cpuState, sourceRegister);
    storeByteInMemory(cpuState, destinationAddress, memoryValue);
}

void addToRegister(CpuState *cpuState, Register sourceRegister, unsigned char value) {
    unsigned char sourceRegisterValue = readFromRegister(cpuState, sourceRegister);
    unsigned char result = (sourceRegisterValue + value) & 0xFF;

    unsigned char flagResult = 0;
    if (result == 0) flagResult |= 0x80; // Flag Z
    if ((sourceRegisterValue & 0x0F) + (value & 0x0F) > 0x0F) flagResult |= 0x20; // Flag H
    if (sourceRegisterValue + value > 0xFF) flagResult |= 0x10; // Flag C

    storeInRegister(cpuState, sourceRegister, result);
    storeInRegister(cpuState, registerF, flagResult);
}

void addToRegisterWithCarry(CpuState *cpuState, Register sourceRegister, unsigned char value) {
    bool isCarryFlagSet = getCFlag(cpuState);
    unsigned char carry = isCarryFlagSet ? 1 : 0;
    addToRegister(cpuState, sourceRegister, value + carry);
}

void addToRegisterPair(CpuState *cpuState, RegisterPair sourceRegisterPair, unsigned short value) {
    unsigned short sourceRegisterPairValue = readFromRegisterPair(cpuState, sourceRegisterPair);
    unsigned short result = (sourceRegisterPairValue + value) & 0xFFFF;

    unsigned char flagResult = 0;
    if ((sourceRegisterPairValue & 0xFFF) + (value & 0xFFF) > 0xFFF) flagResult |= 0x20;// Flag H
    if (sourceRegisterPairValue + value > 0xFFFF) flagResult |= 0x10; // Flag C

    storeInRegisterPair(cpuState, sourceRegisterPair, result);
    storeInRegister(cpuState, registerF, flagResult);
}

void compareRegister(CpuState *cpuState, Register sourceRegister, unsigned char value) {
    unsigned char sourceRegisterValue = readFromRegister(cpuState, sourceRegister);

    unsigned char flagResult = 0;
    if (sourceRegisterValue == value) flagResult |= 0x80; // Flag Z
    flagResult |= 0x40; // Flag N
    if ((sourceRegisterValue & 0x0F) < (value & 0x0F)) flagResult |= 0x20; // Flag H
    if (sourceRegisterValue < value) flagResult |= 0x10; // Flag C

    storeInRegister(cpuState, registerF, flagResult);
}

void logicalAndWithRegister(CpuState *cpuState, Register sourceRegister, unsigned char value) {
    unsigned char sourceRegisterValue = readFromRegister(cpuState, sourceRegister);
    unsigned char result = (sourceRegisterValue & value);

    unsigned char flagResult = 0;
    if (result == 0) flagResult |= 0x80; // Flag Z
    flagResult |= 0x20; // Flag H

    storeInRegister(cpuState, sourceRegister, result);
    storeInRegister(cpuState, registerF, flagResult);
}

void logicalOrWithRegister(CpuState *cpuState, Register sourceRegister, unsigned char value) {
    unsigned char sourceRegisterValue = readFromRegister(cpuState, sourceRegister);
    unsigned char result = (sourceRegisterValue | value);

    unsigned char flagResult = 0;
    if (result == 0) flagResult |= 0x80; //Flag Z

    storeInRegister(cpuState, sourceRegister, result);
    storeInRegister(cpuState, registerF, flagResult);
}

void logicalExclusiveOrWithRegister(CpuState *cpuState, Register sourceRegister, unsigned char value) {
    unsigned char sourceRegisterValue = readFromRegister(cpuState, sourceRegister);
    unsigned char result = (sourceRegisterValue ^ value);

    unsigned char flagResult = 0;
    if (result == 0) flagResult |= 0x80; //Flag Z

    storeInRegister(cpuState, sourceRegister, result);
    storeInRegister(cpuState, registerF, flagResult);
}

void incrementRegister(CpuState *cpuState, Register sourceRegister) {
    unsigned char sourceRegisterValue = readFromRegister(cpuState, sourceRegister);
    unsigned char result = (sourceRegisterValue + 1) & 0xFF;

    unsigned char flagResult = 0;
    if (result == 0) flagResult |= 0x80; // Flag Z
    if ((sourceRegisterValue & 0x0F) + 1 > 0x0F) flagResult |= 0x20; // Flag H

    storeInRegister(cpuState, sourceRegister, result);
    storeInRegister(cpuState, registerF, flagResult);
}

void incrementRegisterPair(CpuState *cpuState, RegisterPair sourceRegisterPair) {
    unsigned short sourceRegisterPairValue = readFromRegisterPair(cpuState, sourceRegisterPair);
    unsigned short result = (sourceRegisterPairValue + 1) & 0xFFFF;
    storeInRegisterPair(cpuState, sourceRegisterPair, result);
}

void incrementMemoryByte(CpuState *cpuState, RegisterPair registerPair) {
    unsigned short address = readFromRegisterPair(cpuState, registerPair);
    unsigned char memoryValue = readByteFromMemory(cpuState, address);
    unsigned char result = (memoryValue + 1) & 0xFF;

    unsigned char flagResult = 0;
    if (result == 0) flagResult |= 0x80; // Flag Z
    if ((memoryValue & 0x0F) + 1 > 0x0F) flagResult |= 0x20; // Flag H

    storeByteInMemory(cpuState, address, result);
    storeInRegister(cpuState, registerF, flagResult);
}

void decrementRegister(CpuState *cpuState, Register sourceRegister) {
    unsigned char sourceRegisterValue = readFromRegister(cpuState, sourceRegister);
    unsigned char result = sourceRegisterValue - 1;

    unsigned char flagResult = 0;
    if (result == 0) flagResult |= 0x80; // Flag Z
    flagResult |= 0x40; // Flag N
    if ((sourceRegisterValue & 0x0F) == 0) flagResult |= 0x20; // Flag H

    storeInRegister(cpuState, sourceRegister, result);
    storeInRegister(cpuState, registerF, flagResult);
}

void decrementRegisterPair(CpuState *cpuState, RegisterPair sourceRegisterPair) {
    unsigned short sourceRegisterPairValue = readFromRegisterPair(cpuState, sourceRegisterPair);
    unsigned short result = (sourceRegisterPairValue - 1) & 0xFFFF;
    storeInRegisterPair(cpuState, sourceRegisterPair, result);
}

void decrementMemoryByte(CpuState *cpuState, RegisterPair registerPair) {
    unsigned short address = readFromRegisterPair(cpuState, registerPair);
    unsigned char memoryValue = readByteFromMemory(cpuState, address);
    unsigned char result = memoryValue - 1;

    unsigned char flagResult = 0;
    if (result == 0) flagResult |= 0x80; // Flag Z
    flagResult |= 0x40; // Flag N
    if ((memoryValue & 0x0F) == 0) flagResult |= 0x20; // Flag H

    storeByteInMemory(cpuState, address, result);
    storeInRegister(cpuState, registerF, flagResult);
}

bool getZFlag(CpuState *cpuState) {
    return (readFromRegister(cpuState, registerF) & 0x80) == 0x80;
}

bool getCFlag(CpuState *cpuState) {
    return (readFromRegister(cpuState, registerF) & 0x10) == 0x10;
}

void pushRegisterPair(CpuState *cpuState, RegisterPair registerPair) {
    cpuState->registers.stackPointer--;
    loadSourceRegisterInMemory(cpuState, registerPair.first, cpuState->registers.stackPointer);
    cpuState->registers.stackPointer--;
    loadSourceRegisterInMemory(cpuState, registerPair.second, cpuState->registers.stackPointer);
    runExtraMachineCycle(cpuState);
}

void popRegisterPair(CpuState *cpuState, RegisterPair registerPair) {
    loadMemoryByteInDestinationRegister(cpuState, cpuState->registers.stackPointer, registerPair.second);
    cpuState->registers.stackPointer++;
    loadMemoryByteInDestinationRegister(cpuState, cpuState->registers.stackPointer, registerPair.first);
    cpuState->registers.stackPointer++;
}

void stackReturn(CpuState *cpuState) {
    cpuState->registers.programCounter = readWordFromMemory(cpuState, cpuState->registers.stackPointer);
    cpuState->registers.stackPointer += 2;
}

void restart(CpuState *cpuState, unsigned short newAddress) {
    cpuState->registers.stackPointer -= 2;
    storeWordInMemory(cpuState, cpuState->registers.stackPointer, cpuState->registers.programCounter);
    cpuState->registers.programCounter = newAddress;
}

void handleIllegalOpcode(unsigned char opcode) {
    fprintf(stderr, "Encountered illegal opcode 0x%X!\n", opcode);
    exit(EXIT_FAILURE);
}