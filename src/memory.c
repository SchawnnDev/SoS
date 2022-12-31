#include <stddef.h>
#include "memory.h"

int memoryCurrentStackOffset = 0;
MemorySpace memory = NULL;

MemorySpace reserveMemorySpace() {
    if (memory == NULL)
        return (memory = newMemorySlot());

    MemorySpace mem = NULL;

    do {
        mem = mem == NULL ? memory : mem->next;
        if (!mem->used)
            return mem;
    } while (mem->next != NULL);

    mem->next = newMemorySlot();
    mem->next->used = true;

    return mem;
}

MemorySpace newMemorySlot() {
    MemorySpace space;
    CHECKPOINTER(space = malloc(sizeof(struct memory_space_t)))

    asm_allocateMemoryOnStack(1);

    space->used = false;
    space->offset = memoryCurrentStackOffset;
    space->next = NULL;
    memoryCurrentStackOffset += ASM_INTEGER_SIZE;

    return space;
}

MemorySpace searchByOffset(int offset) {
    MemorySpace mem = memory;
    while (mem != NULL) {
        if (mem->offset == offset) return mem;
        mem = mem->next;
    }
    return NULL;
}

int getMipsOffset(MemorySpace space) {
    return space == NULL ? RETURN_FAILURE : memoryCurrentStackOffset - space->offset;
}

void destroyMemory() {
    if (memory == NULL)
        return;

    MemorySpace mem = memory;
    MemorySpace temp;

    do {
        temp = mem->next;
        free(mem);
        mem = temp;
    } while (mem != NULL);

    memory = NULL;

}

void freeMemory(MemorySpace mem)
{
    if(mem == NULL) return;
    mem->used = false;
}