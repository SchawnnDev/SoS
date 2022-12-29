#include "asm.h"

char *idToLabel(const char *id)
{
    char *result;
    const unsigned int len = strlen(id) + 2;
    CHECKPOINTER(result = malloc(len))
    CHECKPOINTER(strncpy(result, id + 1, len));
    result[0] = '_';
    return result;
}

int asm_syscall(syscall_t type)
{
    log_trace("asm_syscall of type %s", stringFromSyscall(type))
    asm_code_printf("li $v0, %d\n", type)
    asm_code_printf("syscall\n")
    return RETURN_SUCCESS;
}

int asm_writeAsciiz(const char *label, const char *content)
{
    log_trace("asm_writeAsciiz: %s\n", content)
    asm_data_printf("\t%s: .asciiz %s\n", label, content)
    return RETURN_SUCCESS;
}

int asm_writeStaticArray(const char *label, int size)
{
    asm_data_printf("\t%s: .space %d\n", label, size * ASM_INTEGER_SIZE)
    asm_data_printf("\t\t.word 0");
    for (int i = 1; i < size; ++i)
        asm_data_printf(",0")
    asm_data_printf("\n")
    // TODO: heap management
    return RETURN_SUCCESS;
}

