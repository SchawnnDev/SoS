#ifndef SOS_ASM_H
#define SOS_ASM_H

#include <errno.h>
#include "compilation.h"

#define asm_code_printf(...) \
    do { \
        char buf[ASM_PRINTF_BUF_MAX]; \
        if(snprintf(buf, ASM_PRINTF_BUF_MAX, __VA_ARGS__) < 0) { \
            log_error(strerror(errno));  \
            return RETURN_FAILURE; \
        } \
        addIntoCode(listInstruction, buf); \
    } while (0);

#define asm_data_printf(...) \
    do { \
        char buf[ASM_PRINTF_BUF_MAX]; \
        if(snprintf(buf, ASM_PRINTF_BUF_MAX, __VA_ARGS__) < 0) { \
            log_error(strerror(errno));  \
            return RETURN_FAILURE; \
        } \
        addIntoData(listInstruction, buf); \
    } while (0);

extern ListInstruction listInstruction;

typedef enum
{
    PRINT_INT = 1, // reads $a0
    PRINT_STRING = 4, // reads $a0
    READ_INT = 5, // writes $v0
    READ_STRING = 6, // writes $a0 = buffer, $a1 = length
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

static const char *syscallStrings[] = {
        "print_int",
        "print_string",
        "read_int",
        "read_string",
        "sbrk",
        "exit",
        "print_char",
        "read_char",
        "exit2"
};

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

void asm_readFromStack(const char *into, int what);

int asm_syscall(syscall_t type);

/**
 *
 * @param label
 * @param content
 * @return
 */
int asm_writeAsciiz(const char* label, const char* content);

#endif //SOS_ASM_H
