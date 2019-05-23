#include "cpu.h"
#include "constants.h"

#ifndef MICROOPS_H
#define MICROOPS_H

unsigned char readByteFromMemory(CpuState*, unsigned short);
void storeInRegister(CpuState*, Register, unsigned char);

#endif