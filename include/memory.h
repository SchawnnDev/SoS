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
 * @brief Searches a free memory space, if no space available, creates a new one
 *
 * @param MemorySlot : memory, the memory structure in which reserve a space
 * @param int : memoryCurrentStackOffset, offset needed to create a new memory slot
 *
 * @return MemorySlot, the memorySlot reserved
 */
MemorySlot reserveMemorySlot(MemorySlot memory, int *memoryCurrentStackOffset);

/**
 * @param int : memoryCurrentStackOffset, offset needed to create a new memory slot
 *
 * @return MemorySlot, a new memory slot
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
 * @param MemorySlot : space, the memory structure of the stack
 * @param int : memoryCurrentStackOffset, offset needed to create a new memory slot
 *
 * @return int, current stack offset
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
 * @brief Destroy and free the memory structure
 *
 * @param MemorySlot : memory, the memory to destroy
 */
void destroyMemorySlot(MemorySlot memory);

/**
 * @brief "Free" a part of the structure, in a sense where it is not
 *        used anymore, but isn't freed in a C point of view.
 *
 * @param MemorySlot : memory, the memory slot to free
 */
void freeMemory(MemorySlot memory);

typedef struct list_memory_space_t *MemorySlotList;

/**
 * @brief Structure to organize memorySlots in a list format.
 */
struct list_memory_space_t {
    MemorySlot slot;
    MemorySlotList next;
    MemorySlotList previous;
};

/**
 * @brief Creates a new memorySlotList
 *
 * @param MemorySlot : memorySlot, the memorySlot contained in the list
 * @return MemorySlotList, the new memorySlotList
 */
MemorySlotList newMemorySlotList(MemorySlot memorySlot);

/**
 * @brief Append a memorySlot at the end of the list.
 *
 * @param MemorySlotList : memorySlotList
 * @param MemorySlot : slot
 *
 * @return MemorySlotList, the list appended
 */
MemorySlotList appendMemorySlot(MemorySlotList memorySlotList, MemorySlot slot);

/**
 * @brief Return the first element of the list
 *
 * @param MemorySlotList : memorySlotList
 *
 * @return MemorySlotList, first element of the list
 */
MemorySlotList firstMemorySlotList(MemorySlotList memorySlotList);

/**
 * @brief Destroys and frees the list.
 *
 * @param MemorySlotList : memorySlotList, the list to destroy
 *
 */
void destroyMemoryList(MemorySlotList memorySlotList);

#endif //SOS_MEMORY_H
