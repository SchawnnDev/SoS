#include "asm.h"
#include "atoi.asm.h"

int asm_fctAtoiWritten = FALSE;

int asm_writeAtoiFunction()
{
    if(asm_fctAtoiWritten)
        return RETURN_SUCCESS;
    asm_fctAtoiWritten = TRUE;

    asm_code_printf("\t%s:", ASM_ATOI_FUNCTION_NAME)

    return RETURN_SUCCESS;
}