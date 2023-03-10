#include "asm.h"

char *idToLabel(const char *id)
{
    char *result;
    const unsigned int len = strlen(id) + 2;
    CHECKPOINTER(result = malloc(len))
    CHECK_ERROR_RETURN(NULL)
    CHECKPOINTER(strncpy(result, id + 1, len))
    CHECK_ERROR_RETURN(NULL)
    result[0] = '_';
    return result;
}


int asm_addArgumentsOnStack(int size, ...)
{
    va_list args;
    va_start(args, size);

    // TODO: _offset?

    asm_allocateMemoryOnStack(size * ASM_INTEGER_SIZE);
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    for (int i = 0; i < size; i++)
    {
        char *reg = va_arg(args, char *);
        asm_code_printf("\tsw %s, %d($sp)\n", reg, i * ASM_INTEGER_SIZE);
    }

    va_end(args);

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_syscall(syscall_t type)
{
    log_trace("asm_syscall of type %s", stringFromSyscall(type))
    asm_code_printf("\tli $v0, %d\n", type)
    asm_code_printf("\tsyscall\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_writeAsciiz(const char *label, const char *content, int addQuotes)
{
    log_trace("asm_writeAsciiz: %s\n", content)
    asm_data_printf(addQuotes ? "\t%s: .asciiz \"%s\"\n" : "\t%s: .asciiz %s\n", label, content)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_writeStaticArray(const char *label, int size)
{
    // -1 => value not allocated
    if(size < 1)
    {
        setErrorFailure();
        return RETURN_FAILURE;
    }

    asm_data_printf("\t%s: .word -1", label)
    for (int i = 1; i < size; ++i)
        asm_data_printf(",-1")
    asm_data_printf("\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_readFromStack(const char *into, int offset) {
    asm_getStackAddress(into, offset);
    asm_code_printf("\tlw %s, 0(%s)\n", into, into)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_getStackAddress(const char *into, int offset) {
    // asm_code_printf("\tlw %s, %s\n", into, ASM_VAR_OFFSET_NAME)
    asm_code_printf("\tadd %s, $sp, $s7\n", into)
    asm_code_printf("\taddi %s, %s, %d\n", into, into, offset)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_allocateMemoryOnStack(int words)
{
    asm_code_printf("\taddi $sp, $sp, -%d\n", words * ASM_INTEGER_SIZE)
    //asm_code_printf("\taddi %s, $sp, %d\n", reg, words * ASM_INTEGER_SIZE)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_freeMemoryOnStack(int words)
{
    asm_code_printf("\taddi $sp, $sp, %d\n", words * ASM_INTEGER_SIZE)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_loadLabelAddressIntoRegister(const char *label, const char *reg)
{
    asm_code_printf("\tla %s, %s\n", reg, label)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_loadLabelIntoRegister(const char *label, const char *reg)
{
    asm_code_printf("\tlw %s, %s\n", reg, label)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_jal(const char* name)
{
    asm_code_printf("\tjal %s\n", name)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

// functions

int asm_useBufferWriteFunction(const char* source, const char* destination, const char* into)
{
    asm_code_printf("\tmove $a0, %s\n", source)
    asm_code_printf("\tmove $a1, %s\n", destination)
    asm_jal(ASM_BUFFER_WRITE_FUNCTION_NAME);
    asm_code_printf("\tmove %s, $v0\n", into)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_useBufferLenFunction(const char *bufStartAddressRegister, const char *into)
{
    asm_code_printf("\tmove $a0, %s\n", bufStartAddressRegister)
    asm_jal(ASM_BUFFER_LEN_FUNCTION_NAME);
    asm_code_printf("\tmove %s, $v0\n", into)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_useIntToStringFunction(const char *intAddressRegister, const char*into)
{
    asm_code_printf("\tmove $a0, %s\n", intAddressRegister)
    asm_jal(ASM_INT_TO_STRING_FUNCTION_NAME);
    asm_code_printf("\tmove %s, $v0\n", into)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_useStrCmpFunction(const char *leftStrAddressRegister, const char *rightStrAddressRegister, const char * into)
{
    asm_code_printf("\tmove $a0, %s\n", leftStrAddressRegister)
    asm_code_printf("\tmove $a1, %s\n", rightStrAddressRegister)
    asm_jal(ASM_STRCMP_FUNCTION_NAME);
    asm_code_printf("\tmove %s, $v0\n", into)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_useAtoiFunction(const char *strAddressRegister, const char * into)
{
    asm_code_printf("\tmove $a0, %s\n", strAddressRegister)
    asm_jal(ASM_ATOI_FUNCTION_NAME);
    asm_code_printf("\tmove %s, $v0\n", into)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_appendInternalOffset(int words)
{
    if(words == 0) return RETURN_SUCCESS;
    asm_code_printf("\tla $t0, %s\n", ASM_VAR_OFFSET_NAME)
    asm_code_printf("\tlw $t1, 0($t0)\n")
    asm_code_printf("\taddi $t1, $t1, %d\n", words * ASM_INTEGER_SIZE)
    asm_code_printf("\tsw $t1, 0($t0)\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_subtractInternalOffset(int words)
{
    if(words == 0) return RETURN_SUCCESS;
    asm_code_printf("\tla $t0, %s\n", ASM_VAR_OFFSET_NAME)
    asm_code_printf("\tlw $t1, 0($t0)\n")
    asm_code_printf("\tsubi $t1, $t1, %d\n", words * ASM_INTEGER_SIZE)
    asm_code_printf("\tsw $t1, 0($t0)\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_addIntOnStack(int value)
{
    asm_appendInternalOffset(1);
    asm_allocateMemoryOnStack(1);
    asm_code_printf("\tli $t0, %d\n", value)
    asm_code_printf("\tsw $t0, 0($sp)\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_allocateOnHeap(const char* into, int size)
{
    asm_code_printf("\tli $a0, %d\n", size)
    asm_syscall(SBRK);
    asm_code_printf("\tmove %s, $v0\n", into)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_writeEmptyCharToLastEcho()
{
    asm_loadLabelIntoRegister(ASM_VAR_FCT_RETURN_VALUE, "$t0");
    asm_code_printf("\tli $a0, 1\n")
    asm_syscall(SBRK);
    asm_code_printf("\tsw $zero, 0($v0)\n")
    asm_code_printf("\tsw $v0, 0($t1)\n")
    return RETURN_SUCCESS;
}

int asm_writeArgsToStack()
{ // TODO: TO FUNCTION

    // 2 DATA variables :
    //  - ASM_VAR_ARGV_START >> start pointer of args on stack
    //  - ASM_VAR_ARGC >> number of args given

    // $a0 = argc, $a1 = argv
    asm_code_printf("\tble $a0, $zero, _main\n")
    asm_code_printf("\tla $t0, %s\n", ASM_VAR_ARGC)
    asm_code_printf("\tsw $a0, 0($t0)\n") // sets argv to ASM_VAR_ARGC

    asm_code_printf("\tla $t0, %s\n", ASM_VAR_ARGV_START)
    asm_code_printf("\tsw $a1, 0($t0)\n") // sets argv to ASM_VAR_ARGC

    asm_code_printf("\tj _main\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}


int asm_writeRegistersToStack()
{
    // Save registers to stack
    asm_code_printf("\t\taddi $sp, $sp, -4\n")
    asm_code_printf("\t\tsw $ra, 0($sp)\n")
    asm_code_printf("\t\tjal %s\n", ASM_SAVE_REGISTERS_TO_STACK_FUNCTION_NAME)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_loadRegistersFromStack()
{
    // Load registers from stack
    asm_code_printf("\t\tjal %s\n", ASM_LOAD_REGISTERS_FROM_STACK_FUNCTION_NAME)
    asm_code_printf("\t\tlw $ra, 0($sp)\n")
    asm_code_printf("\t\taddi $sp, $sp, 4\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}