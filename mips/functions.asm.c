#include "functions.asm.h"

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