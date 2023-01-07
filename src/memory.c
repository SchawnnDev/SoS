#include <stddef.h>
#include "memory.h"

MemorySlot reserveMemorySlot(MemorySlot memory, int *memoryCurrentStackOffset)
{
    MemorySlot mem = NULL;

    if (memory == NULL)
    {
        memory = newMemorySlot(memoryCurrentStackOffset);
        memory->used = true;
        return memory;
    }

    do
    {
        mem = mem == NULL ? memory : mem->next;
        if (!mem->used)
        {
            mem->used = true;
            return mem;
        }
    } while (mem->next != NULL);

    mem->next = newMemorySlot(memoryCurrentStackOffset);
    mem->next->used = true;

    return mem->next;
}

MemorySlot newMemorySlot(int *memoryCurrentStackOffset)
{
    log_trace("newMemorySlot()")
    MemorySlot space;
    CHECKPOINTER(space = malloc(sizeof(struct memory_space_t)))
    asm_allocateMemoryOnStack(1);

    space->used = false;
    space->offset = *memoryCurrentStackOffset;
    space->next = NULL;
    space->label = NULL;
    *memoryCurrentStackOffset += ASM_INTEGER_SIZE;

    return space;
}

int getMipsOffset(MemorySlot space, int memoryCurrentStackOffset)
{
    return space == NULL ? RETURN_FAILURE : memoryCurrentStackOffset -
                                            space->offset;
}

void destroyMemorySlot(MemorySlot memory)
{
    if (memory == NULL)
        return;

    MemorySlot mem = memory;
    MemorySlot temp;
    int i = 0;

    do
    {
        i++;
        temp = mem->next;
        free(mem);
        mem = temp;
    } while (mem != NULL);

    asm_freeMemoryOnStack(i);

}

void freeMemory(MemorySlot mem)
{
    if (mem == NULL) return;
    mem->used = false;
}

// MemorySlotList

/**
 *
 * @return
 */
MemorySlotList newMemorySlotList(MemorySlot memorySlot)
{
    if (memorySlot == NULL) return NULL;
    log_trace("newMemorySlotList(%d)", memorySlot->offset)
    MemorySlotList list;
    CHECKPOINTER(list = malloc(sizeof(struct list_memory_space_t)));
    list->slot = memorySlot;
    list->next = NULL;
    list->previous = NULL;
    return list;
}

MemorySlotList appendMemorySlot(MemorySlotList memorySlotList, MemorySlot slot)
{
    log_trace("appendMemorySlot(%d)", slot->offset)
    if (memorySlotList == NULL)
    {
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

    do
    {
        temp = memorySlotList->next;
        free(memorySlotList);
        memorySlotList = temp;
    } while (memorySlotList != NULL);

}

MemorySlotList firstMemorySlotList(MemorySlotList memorySlotList)
{
    if (memorySlotList == NULL) return NULL;
    MemorySlotList result = memorySlotList;

    while (result->previous != NULL)
        result = result->previous;

    return result;
}