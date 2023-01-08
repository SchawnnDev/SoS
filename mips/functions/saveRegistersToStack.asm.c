#include "saveRegistersToStack.asm.h"

int asm_fctSaveRegistersToStackWritten = FALSE;

int asm_writeSaveRegistersToStackFunction()
{
    if (asm_fctSaveRegistersToStackWritten)
        return RETURN_SUCCESS;
    asm_fctSaveRegistersToStackWritten = TRUE;

    const int tempRegisters = 8;
    const int savedRegisters = 8;

    asm_code_printf("\t%s:\n", ASM_SAVE_REGISTERS_TO_STACK_FUNCTION_NAME)
    asm_code_printf("\t\taddi $sp, $sp, -%d\n", (tempRegisters + savedRegisters) * ASM_INTEGER_SIZE)

    int count = 0;

    for (int i = 0; i < tempRegisters; ++i)
    {
        asm_code_printf("\t\tsw $t%d, %d($sp)\n", i, count)
        count += 4;
    }

    for (int i = 0; i < savedRegisters; ++i)
    {
        asm_code_printf("\t\tsw $s%d, %d($sp)\n", i, count)
        count += 4;
    }

    asm_code_printf("\t\taddi $sp, $sp, -%d\n", ASM_INTEGER_SIZE)

    asm_code_printf("\t\tjr $ra\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}
