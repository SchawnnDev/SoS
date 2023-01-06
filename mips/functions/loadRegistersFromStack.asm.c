#include "loadRegistersFromStack.asm.h"

int asm_fctLoadRegistersFromStackWritten = FALSE;

int asm_writeLoadRegistersFromStackFunction()
{
    if(asm_fctLoadRegistersFromStackWritten)
        return RETURN_SUCCESS;
    asm_fctLoadRegistersFromStackWritten = TRUE;
    asm_code_printf("\t%s:\n", ASM_LOAD_REGISTERS_FROM_STACK_FUNCTION_NAME)
    int count = 0;

    asm_subtractInternalOffset(16 + 1); // +1 is $ra

    for (int i = 0; i < 7; ++i)
    {
        asm_code_printf("\t\tlw $t%d, %d($sp)\n", i, count)
        count += 4;
    }

    for (int i = 0; i < 7; ++i)
    {
        asm_code_printf("\t\tlw $s%d, %d($sp)\n", i, count)
        count += 4;
    }

    asm_code_printf("\t\taddi $sp, $sp, 64\n")
    asm_code_printf("\t\tjr $ra\n")

    return RETURN_SUCCESS;
}
