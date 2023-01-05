#include <stddef.h>
#include "memory.h"

int memoryCurrentStackOffset = 0;
MemorySlot memory = NULL;

MemorySlot reserveMemorySlot() {
    MemorySlot mem = NULL;

    if (memory == NULL)
    {
        mem = newMemorySlot();
        mem->used = true;
        return mem;
    }

    do {
        mem = mem == NULL ? memory : mem->next;
        if (!mem->used)
        {
            mem->used = true;
            return mem;
        }
    } while (mem->next != NULL);

    mem->next = newMemorySlot();
    mem->next->used = true;

    return mem;
}

MemorySlot newMemorySlot() {
    log_trace("newMemorySlot()")
    MemorySlot space;
    CHECKPOINTER(space = malloc(sizeof(struct memory_space_t)))

    asm_allocateMemoryOnStack(1);

    space->used = false;
    space->offset = memoryCurrentStackOffset;
    space->next = NULL;
    space->temp = true;
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
    log_trace("newMemorySlotList(%d)", memorySlot->offset)
    MemorySlotList list;
    CHECKPOINTER(list = malloc(sizeof (struct list_memory_space_t)));
    list->slot = memorySlot;
    list->next = NULL;
    list->previous = NULL;
    return list;
}

MemorySlotList appendMemorySlot(MemorySlotList memorySlotList, MemorySlot slot)
{
    log_trace("appendMemorySlot(%d)", slot->offset)
    if(memorySlotList == NULL) {
        return NULL;
    }
    MemorySlotList m = newMemorySlotList(slot);
    m->previous = memorySlotList;
    memorySlotList->next = m;

    return m;
}

void destroyMemoryList(MemorySlotList memorySlotList)
{
    if (memorySlotList == NULL)
        return;

    memorySlotList = firstMemorySlotList(memorySlotList);
    MemorySlotList temp;

    do {
        temp = memorySlotList->next;
        free(memorySlotList);
        memorySlotList = temp;
    } while (memorySlotList != NULL);

}

MemorySlotList firstMemorySlotList(MemorySlotList memorySlotList)
{
    if(memorySlotList == NULL) return NULL;
    MemorySlotList result = memorySlotList;

    while(result->previous != NULL)
        result = result->previous;

    return result;
}