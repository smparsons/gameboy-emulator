#include "cpu.h"
#include "constants.h"

#ifndef MICROOPS_H
#define MICROOPS_H

unsigned char readByteFromMemory(CpuState*, unsigned short);
unsigned char storeByteInMemory(CpuState*, unsigned short, unsigned char);
void storeInRegister(CpuState*, Register, unsigned char);
unsigned char readFromRegister(CpuState*, Register);

#endif