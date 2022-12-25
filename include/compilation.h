#ifndef SOS_COMPILATION_H
#define SOS_COMPILATION_H

#include <stdarg.h>

#include "listRangeVar.h"

#define CHECK_TYPE(op) do { if (((op) != 1)) {log_error("not good type."); return EXIT_FAILURE; } } while (0);
#define asm_fprintf(...) \
    do { \
        if (fprintf(outFile, __VA_ARGS__) < 0) { \
            log_error(strerror(errno)); \
            return RETURN_FAILURE; \
        }                \
    } while (0);

ListRangeVariable listRangeVariable;
ListIdentifierOrder listIdentifierOrder;
ListTmp listTmp;
FILE *outFile;

void initStruct();

void addIdOrder(char *name);

void setTypeOrder(int type);

/*!
 * \fn void addTmpValuesListTmp
 * \brief Fonction qui ajoute une structure de valeur temporaire
*/
void addTmpValuesListTmp();

void assign();

void addValueIntoListTmp(char *value);

void echo();

void multiplyFromTmp();

int compile(FILE *inputFile, FILE *outputFile);

/**
 *
 * @param pattern
 * @param string
 * @return -1 if error, 0 if not matching, 1 if matching
 */
int checkRegex(const char *pattern, const char *string);

// Functions

int checkWordIsId(const char *word);

int checkWordIsInt(const char *word);

int parseInt32(const char *word);

/*
 * MIPS GENERATION
 */

typedef enum {
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

static inline const char *stringFromSyscall(syscall_t syscall) {
    return syscallStrings[syscall];
}

// Boolean expression
int asm_printEquals(const char *id1, const char *id2);

int asm_printNotEquals(const char *id1, const char *id2);

int asm_syscall(syscall_t type);

#endif //SOS_COMPILATION_H
