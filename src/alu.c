#include <stdbool.h>
#include "constants.h"
#include "cpu.h"
#include "microops.h"

bool getZFlag(CpuState *cpuState) {
    return (readFromRegister(cpuState, registerF) & 0x80) == 0x80;
}

bool getCFlag(CpuState *cpuState) {
    return (readFromRegister(cpuState, registerF) & 0x10) == 0x10;
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

void subtractFromRegister(CpuState *cpuState, Register sourceRegister, unsigned char value) {
    unsigned char sourceRegisterValue = readFromRegister(cpuState, sourceRegister);
    unsigned char result = (sourceRegisterValue - value) & 0xFF;

    unsigned char flagResult = 0;
    if (sourceRegisterValue == value) flagResult |= 0x80; // Flag Z
    flagResult |= 0x40; // Flag N
    if ((sourceRegisterValue & 0x0F) < (value & 0x0F)) flagResult |= 0x20; // Flag H
    if (sourceRegisterValue < value) flagResult |= 0x10; // Flag C

    storeInRegister(cpuState, sourceRegister, result);
    storeInRegister(cpuState, registerF, flagResult);
}

void subtractFromRegisterWithCarry(CpuState *cpuState, Register sourceRegister, unsigned char value) {
    bool isCarryFlagSet = getCFlag(cpuState);
    unsigned char carry = isCarryFlagSet ? 1 : 0;
    subtractFromRegister(cpuState, sourceRegister, value + carry);
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