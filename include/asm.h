#ifndef SOS_ASM_H
#define SOS_ASM_H

#include <errno.h>
#include <stdarg.h>
#include "listInstructionMips.h"

// TODO: fix error code
#define asm_code_printf(...) \
    do { \
        char buf[ASM_PRINTF_BUF_MAX]; \
        if(snprintf(buf, ASM_PRINTF_BUF_MAX, __VA_ARGS__) < 0) { \
            log_error(strerror(errno));  \
            return 0; \
        } \
        addIntoCode(listInstruction, buf); \
    } while (0);

#define asm_data_printf(...) \
    do { \
        char buf[ASM_PRINTF_BUF_MAX]; \
        if(snprintf(buf, ASM_PRINTF_BUF_MAX, __VA_ARGS__) < 0) { \
            log_error(strerror(errno));  \
            return 0; \
        } \
        addIntoData(listInstruction, buf); \
    } while (0);

extern ListInstruction listInstruction;

typedef enum
{
    PRINT_INT = 1, // reads $a0
    PRINT_STRING = 4, // reads $a0
    READ_INT = 5, // writes $v0
    READ_STRING = 8, // writes $a0 = buffer, $a1 = length
    SBRK = 9, // reads $a0 = amount, writes $v0 = address
    SYS_EXIT = 10,
    PRINT_CHAR = 11, // reads $a0 = character
    READ_CHAR = 12, // writes $a0 = character
    SYS_EXIT2 = 17, // reads $a0 = status code

    /*
    open 13 $a0 = filename, file descriptor (in $v0)  $a1 = flags, $a2 = mode
    read 14 $a0 = file descriptor, bytes read (in $v0) $a1 = buffer, $a2 = count
    write 15 $a0 = file descriptor, bytes written (in $v0) $a1 = buffer, $a2 = count
    close 16 $a0 = file descriptor 0 (in $v0)
    */
} syscall_t;

static inline const char *stringFromSyscall(syscall_t syscall)
{
    switch (syscall)
    {
        case PRINT_INT:
            return "print_int";
        case PRINT_STRING:
            return "print_string";
        case READ_INT:
            return "read_int";
        case READ_STRING:
            return "read_string";
        case SBRK:
            return "sbrk";
        case SYS_EXIT:
            return "exit";
        case PRINT_CHAR:
            return "print_char";
        case READ_CHAR:
            return "read_char";
        case SYS_EXIT2:
            return "exit2";
        default:
            return "unknown";
    }
}

/**
 *
 * @param reg
 * @param words
 * @return
 */
int asm_allocateMemoryOnStack(int words);

/**
 * /!\ INCREASES _offset
 * @param value
 * @return
 */
int asm_addIntOnStack(int value);

/**
 *
 * @param count
 * @return
 */
int asm_appendInternalOffset(int count);

/**
 *
 * @param words
 * @return
 */
int asm_subtractInternalOffset(int words);

/**
 *
 * @param into
 * @param offset
 * @return
 */
int asm_readFromStack(const char *into, int offset);

/**
 *
 * @param into
 * @param offset
 * @return
 */
int asm_getStackAddress(const char *into, int offset);

/**
 *
 * @param type
 * @return
 */
int asm_syscall(syscall_t type);

/**
 *
 * @param label
 * @param content
 * @return
 */
int asm_writeAsciiz(const char *label, const char *content, int addQuotes);

/**
 *
 * @param label
 * @param size
 * @return
 */
int asm_writeStaticArray(const char* label, int size);

/**
 *
 * @param reg
 * @return
 */
int asm_useBufferWriteFunction(const char* source, const char* destination, const char* into);

/**
 *
 * @param bufStartAddressRegister
 * @param into
 * @return
 */
int asm_useBufferLenFunction(const char *bufStartAddressRegister, const char *into);

/**
 *
 * @param label
 * @param reg
 * @return
 */
int asm_loadLabelIntoRegister(const char *label, const char* reg);

/**
 *
 * @param name
 * @return
 */
int asm_jal(const char* name);

/**
 *
 * @param size
 * @param ...
 * @return
 */
int asm_addArgumentsOnStack(int size, ...);

/**
 *
 * @param into
 * @param size
 * @return
 */
int asm_allocateOnHeap(const char* into, int size);

// UTILS

/**
 *
 * @param id
 * @return
 */
char* idToLabel(const char* id);

#endif //SOS_ASM_H
