#ifndef SOS_BUFFERWRITE_ASM_H
#define SOS_BUFFERWRITE_ASM_H
#include "asm.h"

/**
 * Writes a buffer to the heap (until
 * it encounters a \0).
 *
 * @return 0 if success, -1 if it fails
 */
int asm_writeBufferWriteFunction();

#endif //SOS_BUFFERWRITE_ASM_H
