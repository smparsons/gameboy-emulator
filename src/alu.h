#include "cpu.h"
#include "constants.h"

#ifndef ALU_H
#define ALU_H

bool getZFlag(CpuState*);
bool getCFlag(CpuState*);
void addToRegister(CpuState*, Register, unsigned char);
void addToRegisterWithCarry(CpuState*, Register, unsigned char);
void addToRegisterPair(CpuState*, RegisterPair, unsigned short);
void compareRegister(CpuState*, Register, unsigned char);
void subtractFromRegister(CpuState*, Register, unsigned char);
void subtractFromRegisterWithCarry(CpuState*, Register, unsigned char);
void logicalAndWithRegister(CpuState*, Register, unsigned char);
void logicalOrWithRegister(CpuState*, Register, unsigned char);
void logicalExclusiveOrWithRegister(CpuState*, Register, unsigned char);
void incrementRegister(CpuState*, Register);
void incrementRegisterPair(CpuState*, RegisterPair);
void incrementMemoryByte(CpuState*, RegisterPair);
void decrementRegister(CpuState*, Register);
void decrementRegisterPair(CpuState*, RegisterPair);
void decrementMemoryByte(CpuState*, RegisterPair);

#endif