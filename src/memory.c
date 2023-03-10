#include <stddef.h>
#include "memory.h"

MemorySlot reserveMemorySlot(MemorySlot memory, int *memoryCurrentStackOffset)
{
    MemorySlot mem = NULL;

    if (memory == NULL)
    {
        memory = newMemorySlot(memoryCurrentStackOffset);
        CHECK_ERROR_RETURN(NULL)
        memory->offset = 0;
        *memoryCurrentStackOffset = 0;
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
    CHECK_ERROR_RETURN(NULL)
    mem->next->used = true;

    return mem->next;
}

MemorySlot newMemorySlot(int *memoryCurrentStackOffset)
{
    log_trace("newMemorySlot()")
    MemorySlot space;
    CHECKPOINTER(space = malloc(sizeof(struct memory_space_t)))
    CHECK_ERROR_RETURN(NULL)
    asm_allocateMemoryOnStack(1);
    asm_code_printf("\taddi $s7, $s7, %d\n", ASM_INTEGER_SIZE)
    CHECK_ERROR_RETURN(NULL)

    *memoryCurrentStackOffset += ASM_INTEGER_SIZE;

    space->used = false;
    space->offset = *memoryCurrentStackOffset;
    space->next = NULL;
    space->label = NULL;
    space->value = NULL;

    return space;
}

int getMipsOffset(MemorySlot space, int memoryCurrentStackOffset) {
    if(space == NULL)
    {
        setErrorFailure();
        return RETURN_FAILURE;
    }
    return -space->offset;
}

int destroyMemorySlot(MemorySlot memory)
{
    if (memory == NULL)
        return RETURN_FAILURE;

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

    //asm_freeMemoryOnStack(i);
    asm_code_printf("\tadd $sp, $sp, $s7\n")
    return RETURN_SUCCESS;
}

void freeMemory(MemorySlot mem)
{
    if (mem == NULL) return;
/*    if(mem->value != NULL)
    {
        free(mem->value);
        mem->value = NULL;
    }*/
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
    CHECKPOINTER(list = malloc(sizeof (struct list_memory_space_t)))
    CHECK_ERROR_RETURN(NULL)
    list->slot = memorySlot;
    list->next = NULL;
    list->previous = NULL;
    return list;
}

MemorySlotList appendMemorySlot(MemorySlotList memorySlotList, MemorySlot slot)
{
    log_trace("appendMemorySlot(%d)", slot->offset)
    if(memorySlotList == NULL) {
        setErrorFailure();
        return NULL;
    }
    MemorySlotList m = newMemorySlotList(slot);
    CHECK_ERROR_RETURN(NULL)
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

MemorySlotList lastMemorySlotList(MemorySlotList memorySlotList)
{
    if (memorySlotList == NULL) return NULL;
    MemorySlotList result = memorySlotList;

    while (result->next != NULL)
        result = result->next;

    return result;
}