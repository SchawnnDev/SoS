#include "saveRegistersToStack.asm.h"

int asm_fctSaveRegistersToStackWritten = FALSE;

int asm_writeSaveRegistersToStackFunction()
{
    if (asm_fctSaveRegistersToStackWritten)
        return RETURN_SUCCESS;
    asm_fctSaveRegistersToStackWritten = TRUE;

    asm_code_printf("\t%s:\n", ASM_SAVE_REGISTERS_TO_STACK_FUNCTION_NAME)
    asm_code_printf("\t\taddi $sp, $sp, -64\n")
    // TODO: fix this ($t0 should not be written in this fct) (maybe add value to stack)
    //asm_appendInternalOffset(16);
    int count = 0;

    for (int i = 0; i < 7; ++i)
    {
        asm_code_printf("\t\tsw $t%d, %d($sp)\n", i, count)
        count += 4;
    }

    for (int i = 0; i < 7; ++i)
    {
        asm_code_printf("\t\tsw $s%d, %d($sp)\n", i, count)
        count += 4;
    }

    asm_code_printf("\t\tjr $ra\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}
