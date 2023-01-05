#ifndef SOS_ERRORMANAGER_ASM_H
#define SOS_ERRORMANAGER_ASM_H
#include "functions.asm.h"

int asm_writeOutOfBoundsErrorFunction();
int asm_writeNotANumberErrorFunction();
int asm_writeArrayElementNotAllocatedErrorFunction();
int asm_writePrintErrorAndExit();

#endif //SOS_ERRORMANAGER_ASM_H
