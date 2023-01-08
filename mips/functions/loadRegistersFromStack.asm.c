#include "loadRegistersFromStack.asm.h"

int asm_fctLoadRegistersFromStackWritten = FALSE;

int asm_writeLoadRegistersFromStackFunction()
{
    if(asm_fctLoadRegistersFromStackWritten)
        return RETURN_SUCCESS;
    asm_fctLoadRegistersFromStackWritten = TRUE;

    asm_code_printf("\t%s:\n", ASM_LOAD_REGISTERS_FROM_STACK_FUNCTION_NAME)
    int count = 0;

    asm_code_printf("\t\taddi $sp, $sp, %d\n", ASM_INTEGER_SIZE)

    const int tempRegisters = 8;
    const int savedRegisters = 8;

    for (int i = 0; i < tempRegisters; ++i)
    {
        asm_code_printf("\t\tlw $t%d, %d($sp)\n", i, count)
        count += 4;
    }

    for (int i = 0; i < savedRegisters; ++i)
    {
        asm_code_printf("\t\tlw $s%d, %d($sp)\n", i, count)
        count += 4;
    }

    asm_code_printf("\t\taddi $sp, $sp, %d\n", (tempRegisters + savedRegisters) * ASM_INTEGER_SIZE)
    asm_code_printf("\t\tjr $ra\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}
