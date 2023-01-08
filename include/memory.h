#ifndef SOS_MEMORY_H
#define SOS_MEMORY_H
#include "asm.h"

typedef struct memory_space_t *MemorySlot;

struct memory_space_t {
    int used;
    int offset;
    char* label; // temp replaced by label, label == null -> temp
    char* value;

    MemorySlot next;
};


/**
 * Searches a free memory space, if no space available, creates a new one
 * @return
 */
MemorySlot reserveMemorySlot(MemorySlot memory, int *memoryCurrentStackOffset);

/**
 *
 * @return
 */
MemorySlot newMemorySlot(int *memoryCurrentStackOffset);

/**
 *
 * @param offset
 * @return
 */
MemorySlot searchByOffset(int offset);

/**
 *
 * @param space
 * @return
 */
int getMipsOffset(MemorySlot space, int memoryCurrentStackOffset);

/**
 *
 * @return
 */
int getMemoryCurrentStackOffset();

/**
 *
 * @param memCurrentStackOffset
 */
void setMemoryCurrentStackOffset(int memCurrentStackOffset);

/**
 *
 * @param offset_target
 * @param appendStack
 */
void expandMemorySlots(int offset_target, bool appendStack);

/**
 *
 */
int destroyMemorySlot(MemorySlot memory);

/**
 *
 * @param memory
 */
void freeMemory(MemorySlot memory);

typedef struct list_memory_space_t *MemorySlotList;

struct list_memory_space_t {
    MemorySlot slot;
    MemorySlotList next;
    MemorySlotList previous;
};

/**
 *
 * @param memorySlot
 * @return
 */
MemorySlotList newMemorySlotList(MemorySlot memorySlot);

/**
 *
 * @param memorySlotList
 * @param slot
 */
MemorySlotList appendMemorySlot(MemorySlotList memorySlotList, MemorySlot slot);

/**
 *
 * @param memorySlotList
 * @return
 */
MemorySlotList firstMemorySlotList(MemorySlotList memorySlotList);

/**
 *
 */
void destroyMemoryList(MemorySlotList memorySlotList);

#endif //SOS_MEMORY_H
