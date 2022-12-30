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


int asm_addArgumentsOnStack(int size, ...)
{
    va_list args;
    va_start(args, size);

    // TODO: _offset?

    asm_allocateMemoryOnStack(size * ASM_INTEGER_SIZE);

    for (int i = 0; i < size; i++)
    {
        char *reg = va_arg(args, char *);
        asm_code_printf("\tsw %s, %d($sp)\n", reg, i * ASM_INTEGER_SIZE);
    }

    va_end(args);


    return RETURN_SUCCESS;
}

int asm_syscall(syscall_t type)
{
    log_trace("asm_syscall of type %s", stringFromSyscall(type))
    asm_code_printf("\tli $v0, %d\n", type)
    asm_code_printf("\tsyscall\n")
    return RETURN_SUCCESS;
}

int asm_writeAsciiz(const char *label, const char *content, int addQuotes)
{
    log_trace("asm_writeAsciiz: %s\n", content)
    asm_data_printf(addQuotes ? "\t%s: .asciiz \"%s\"\n" : "\t%s: .asciiz %s\n", label, content)
    return RETURN_SUCCESS;
}

int asm_writeStaticArray(const char *label, int size)
{
    asm_data_printf("\t%s: .space %d\n", label, size * ASM_INTEGER_SIZE)
    asm_data_printf("\t\t.word 0");
    for (int i = 1; i < size; ++i)
        asm_data_printf(",0")
    asm_data_printf("\n")
    return RETURN_SUCCESS;
}

int asm_readFromStack(const char *into, char *offset) {
    asm_code_printf("\tlw %s, _offset\n", into)
    asm_code_printf("\tadd %s, $sp, %s\n", into, into)
    asm_code_printf("\taddi %s, %s, %s\n", into, into, offset)
    asm_code_printf("\tlw %s, 0(%s)\n", into, into)
    return RETURN_SUCCESS;
}

int asm_allocateMemoryOnStack(int words)
{
    asm_code_printf("\taddi $sp, $sp, -%d\n", words * ASM_INTEGER_SIZE)
    //asm_code_printf("\taddi %s, $sp, %d\n", reg, words * ASM_INTEGER_SIZE)
    return RETURN_SUCCESS;
}

int asm_loadLabelIntoRegister(const char *label, const char *reg)
{
    asm_code_printf("\tla %s, %s\n", reg, label)
    return RETURN_SUCCESS;
}

int asm_jal(const char* name)
{
    asm_code_printf("\tjal %s\n", name)
    return RETURN_SUCCESS;
}

// functions

int asm_useBufferWriteFunction(const char* source, const char* destination, const char* into)
{
    asm_code_printf("\tmove $a0, %s\n", source)
    asm_code_printf("\tmove $a1, %s\n", destination)
    asm_jal(ASM_BUFFER_WRITE_FUNCTION_NAME);
    asm_code_printf("\tmove %s, $v0\n", into)
    return asm_syscall(PRINT_STRING);
}

int asm_useBufferLenFunction(const char *bufStartAddressRegister, const char *into)
{
    asm_code_printf("\tmove $a0, %s\n", bufStartAddressRegister)
    asm_jal(ASM_BUFFER_LEN_FUNCTION_NAME);
    asm_code_printf("\tmove %s, $v0\n", into)
    return RETURN_SUCCESS;
}