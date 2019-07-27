#ifndef CONSTANTS_H
#define CONSTANTS_H

typedef enum Register { 
    registerA, registerB, registerC, registerD, registerE, registerH, registerL, registerF 
} Register;

typedef struct RegisterPair
{
    Register first;
    Register second;
} RegisterPair;

#endif