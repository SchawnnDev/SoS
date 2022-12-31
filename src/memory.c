#include <stddef.h>
#include "memory.h"

int memoryCurrentStackOffset = 0;
MemorySlot memory = NULL;

MemorySlot reserveMemorySlot() {
    if (memory == NULL)
        return (memory = newMemorySlot());

    MemorySlot mem = NULL;

    do {
        mem = mem == NULL ? memory : mem->next;
        if (!mem->used)
            return mem;
    } while (mem->next != NULL);

    mem->next = newMemorySlot();
    mem->next->used = true;

    return mem;
}

MemorySlot newMemorySlot() {
    MemorySlot space;
    CHECKPOINTER(space = malloc(sizeof(struct memory_space_t)))

    asm_allocateMemoryOnStack(1);

    space->used = false;
    space->offset = memoryCurrentStackOffset;
    space->next = NULL;
    memoryCurrentStackOffset += ASM_INTEGER_SIZE;

    return space;
}

MemorySlot searchByOffset(int offset) {
    MemorySlot mem = memory;
    while (mem != NULL) {
        if (mem->offset == offset) return mem;
        mem = mem->next;
    }
    return NULL;
}

int getMipsOffset(MemorySlot space) {
    return space == NULL ? RETURN_FAILURE : memoryCurrentStackOffset - space->offset;
}

void destroyMemorySlot() {
    if (memory == NULL)
        return;

    MemorySlot mem = memory;
    MemorySlot temp;

    do {
        temp = mem->next;
        free(mem);
        mem = temp;
    } while (mem != NULL);

    memory = NULL;

}

void freeMemory(MemorySlot mem)
{
    if(mem == NULL) return;
    mem->used = false;
}

// MemorySlotList

/**
 *
 * @return
 */
MemorySlotList newMemorySlotList(MemorySlot memorySlot)
{
    MemorySlotList list;
    CHECKPOINTER(list = malloc(sizeof (struct list_memory_space_t)));
    list->slot = memorySlot;
    list->next = NULL;
    return list;
}

void appendMemorySlot(MemorySlotList memorySlotList, MemorySlot slot)
{
    // TODO CHANGE HERE FROM BOTTOM TO END ;;;;
    if(memorySlotList == NULL) return;
    memorySlotList->next = newMemorySlotList(slot);
}

void destroyMemoryList(MemorySlotList memorySlotList)
{
    if (memorySlotList == NULL)
        return;

    MemorySlotList temp;

    do {
        temp = memorySlotList->next;
        free(memorySlotList);
        memorySlotList = temp;
    } while (memorySlotList != NULL);

}