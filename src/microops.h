#include "cpu.h"
#include "constants.h"

#ifndef MICROOPS_H
#define MICROOPS_H

unsigned char readByteFromMemory(CpuState*, unsigned short);
unsigned short readWordFromMemory(CpuState*, unsigned short);
void storeByteInMemory(CpuState*, unsigned short, unsigned char);
void storeWordInMemory(CpuState*, unsigned short, unsigned short);
unsigned char readFromRegister(CpuState*, Register);
void storeInRegister(CpuState*, Register, unsigned char);
unsigned short readFromRegisterPair(CpuState*, RegisterPair);
void storeInRegisterPair(CpuState*, RegisterPair, unsigned short);
void runExtraMachineCycle(CpuState*);

#endif