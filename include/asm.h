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
            if(!HAS_ERROR()) setErrorFailure();  \
            return 0; \
        } \
        addIntoCode(listInstruction, buf); \
    } while (0);

#define asm_data_printf(...) \
    do { \
        char buf[ASM_PRINTF_BUF_MAX]; \
        if(snprintf(buf, ASM_PRINTF_BUF_MAX, __VA_ARGS__) < 0) { \
            log_error(strerror(errno));  \
            if(!HAS_ERROR()) setErrorFailure();  \
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
 * @brief allocate memory on the stack
 *
 * @param int : words, number of words
 * @return  0 if success, -1 if it fails
 */
int asm_allocateMemoryOnStack(int words);

/**
 * @brief free memory on the stack
 *
 * @param int : words, number of words to free
 * @return 0 if success, -1 if it fails
 */
int asm_freeMemoryOnStack(int words);

/**
 * @brief add an integer on the stack
 *
 * /!\ INCREASES _offset
 * @param int : value, value of the integer to add
 * @return 0 if success, -1 if it fails
 */
int asm_addIntOnStack(int value);

/**
 * @brief increase the offset of the stack
 *
 * @param int : count, number of words from which increase the stack
 * @return 0 if success, -1 if it fails
 */
int asm_appendInternalOffset(int count);

/**
 * @brief decrease the offset of the stack
 *
 * @param int : words, number of words from which decrease the stack
 * @return 0 if success, -1 if it fails
 */
int asm_subtractInternalOffset(int words);

/**
 * @brief read data from stack at a given offset
 *
 * @param const char* : into, value into which store the read data
 * @param int : offset
 * @return 0 if success, -1 if it fails
 */
int asm_readFromStack(const char *into, int offset);

/**
 * @brief gets the address of the stack at a given offset
 *
 * @param const char* : into, value into which store the address
 * @param int : offset
 * @return 0 if success, -1 if it fails
 */
int asm_getStackAddress(const char *into, int offset);

/**
 * @brief calls a MIPS syscall function
 *
 * @param syscall_t : type, type of syscall called
 * @return 0 if success, -1 if it fails
 */
int asm_syscall(syscall_t type);

/**
 * @brief writes an .asciiz value in the MIPS code
 *
 * @param const char* : label, label of the .asciiz value
 * @param const char* : content, content of the .asciiz value
 * @param int : addQuotes, 1 if quotes are needed, 0 otherwise
 * @return 0 if success, -1 if it fails
 */
int asm_writeAsciiz(const char *label, const char *content, int addQuotes);

/**
 * @brief write a static array
 *
 * @param const char* : label, label of the array
 * @param int : size, size of the array
 * @return 0 if success, -1 if it fails
 */
int asm_writeStaticArray(const char* label, int size);

/**
 * @brief calls the bufferWrite function
 *
 * @param const char* : source
 * @param const char* destination
 * @param const char* into
 * @return 0 if success, -1 if it fails
 */
int asm_useBufferWriteFunction(const char* source, const char* destination, const char* into);

/**
 * @brief calls the bufferLen function
 *
 * @param const char* : bufStartAddressRegister
 * @param const char* : into
 * @return 0 if success, -1 if it fails
 */
int asm_useBufferLenFunction(const char *bufStartAddressRegister, const char *into);

/**
 * @brief calls the intToString function
 *
 * @param const char* : intAddressRegister
 * @param const char* : into
 * @return 0 if success, -1 if it fails
 */
int asm_useIntToStringFunction(const char *intAddressRegister, const char *into);

/**
 * @brief calls the strCmp function
 *
 * @param const char* : leftStrAddressRegister
 * @param const char* : rightStrAddressRegister
 * @param const char* : into
 * @return 0 if success, -1 if it fails
 */
int asm_useStrCmpFunction(const char *leftStrAddressRegister, const char *rightStrAddressRegister, const char * into);

/**
 * @brief calls the atoi function
 *
 * @param const char* : strAddressRegister
 * @param const char* : into
 * @return 0 if success, -1 if it fails
 */
int asm_useAtoiFunction(const char *strAddressRegister, const char * into);

/**
 * @brief calls the loadLabelAddressIntoRegister function
 *
 * @param const char* : label
 * @param const char* : reg, register into which load the address
 * @return 0 if success, -1 if it fails
 */
int asm_loadLabelAddressIntoRegister(const char *label, const char* reg);

/**
 *  Loads label value into register (lw)
 * @param const char* : label
 * @param const char* : reg, register into which load the address
 * @return 0 if success, -1 if it fails
 */
int asm_loadLabelIntoRegister(const char *label, const char *reg);

/**
 * @brief writes a jal instruction in MIPS
 *
 * @param const char* : name, label to which jump
 * @return 0 if success, -1 if it fails
 */
int asm_jal(const char* name);

/**
 *
 * @param size
 * @param ...
 * @return 0 if success, -1 if it fails
 */
int asm_addArgumentsOnStack(int size, ...);

/**
 * @brief allocate on heap
 *
 * @param const char* : into
 * @param int : size
 * @return 0 if success, -1 if it fails
 */
int asm_allocateOnHeap(const char* into, int size);

/**
 * @brief writes the arguments of a function on the stack
 *
 * @return 0 if success, -1 if it fails
 */
int asm_writeArgsToStack();

/**
 * @brief calls the saveRegisterToStack function
 *
 * @return 0 if success, -1 if it fails
 */
int asm_writeRegistersToStack();

/**
 * @brief calls the loadRegisterFromStack function
 *
 * @return 0 if success, -1 if it fails
 */
int asm_loadRegistersFromStack();

// UTILS

/**
 * @brief transforms an id into a usable label
 *
 * @param const char* : id, id to transform
 * @return the label created
 */
char* idToLabel(const char* id);

#endif //SOS_ASM_H
