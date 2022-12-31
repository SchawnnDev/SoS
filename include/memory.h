#ifndef SOS_MEMORY_H
#define SOS_MEMORY_H
#include "asm.h"
typedef struct memory_space_t *MemorySpace;

struct memory_space_t {
    int used;
    int offset;
    // TODO: Heap= true/false, heapOffset for arrays ???
    MemorySpace next;
};

/**
 * Searches a free memory space, if no space available, creates a new one
 * @return
 */
MemorySpace reserveMemorySpace();

/**
 *
 * @return
 */
MemorySpace newMemorySlot();

/**
 *
 * @param offset
 * @return
 */
MemorySpace searchByOffset(int offset);

/**
 *
 * @param space
 * @return
 */
int getMipsOffset(MemorySpace space);

/**
 *
 */
void destroyMemory();

/**
 *
 * @param memory
 */
void freeMemory(MemorySpace memory);

#endif //SOS_MEMORY_H
