#include "displayString.asm.h"

int asm_fctDisplayStringWritten = FALSE;

int asm_writeDisplayStringFunction()
{
    if(asm_fctDisplayStringWritten)
        return RETURN_SUCCESS;
    asm_fctDisplayStringWritten = TRUE;
    asm_code_printf("\t%s:\n", ASM_DISPLAY_STRING_FUNCTION_NAME)
    asm_writeRegistersToStack();
    asm_code_printf("\t\tmove $t0, $a0\n")
    asm_code_printf("\t\t%s_loop:\n", ASM_DISPLAY_STRING_FUNCTION_NAME)
    asm_code_printf("\t\t\tlb $a0, 0($t0)\n")
    asm_code_printf("\t\t\tbeq $a0, $zero, %s_exit\n", ASM_DISPLAY_STRING_FUNCTION_NAME)
    asm_syscall(PRINT_CHAR);
    asm_code_printf("\t\t\taddi $t0, $t0, 4\n")
    asm_code_printf("\t\t\tb %s_loop\n", ASM_DISPLAY_STRING_FUNCTION_NAME)
    asm_code_printf("\t\t%s_exit:\n", ASM_DISPLAY_STRING_FUNCTION_NAME)
    asm_loadRegistersFromStack();
    asm_code_printf("\t\t\tjr $ra\n")
    return RETURN_SUCCESS;
}