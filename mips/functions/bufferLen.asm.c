#include "bufferLen.asm.h"

int asm_fctBufferLenWritten = FALSE;

/*
# Function to count the number of characters in a buffer until the Nul ('\0') character
#
# Arguments:
#   $a0: Pointer to the start of the buffer
#
# Returns:
#   $v0: Number of characters in the buffer until the Nul character
 */
int asm_writeBufferLenFunction()
{
    if(asm_fctBufferLenWritten)
        return RETURN_SUCCESS;
    asm_fctBufferLenWritten = TRUE;
    asm_code_printf("\t%s:\n", ASM_BUFFER_LEN_FUNCTION_NAME)
    asm_writeRegistersToStack();
    asm_code_printf("\t\taddi $t0, $zero, 0\n")
    asm_code_printf("\t\taddi $t1, $zero, 1\n")
    asm_code_printf("\t\t%s_loop:\n", ASM_BUFFER_LEN_FUNCTION_NAME)
    asm_code_printf("\t\t\tlb $t2, 0($a0)\n")
    asm_code_printf("\t\t\tbeq $t2, $zero, %s_exit\n", ASM_BUFFER_LEN_FUNCTION_NAME)
    asm_code_printf("\t\t\taddi $t0, $t0, 1\n")
    asm_code_printf("\t\t\taddi $a0, $a0, 1\n")
    asm_code_printf("\t\t\tb %s_loop\n", ASM_BUFFER_LEN_FUNCTION_NAME)
    asm_code_printf("\t\t%s_exit:\n", ASM_BUFFER_LEN_FUNCTION_NAME)
    asm_code_printf("\t\t\tmove $v0, $t0\n")
    asm_loadRegistersFromStack();
    asm_code_printf("\t\t\tjr $ra\n")
    return RETURN_SUCCESS;
}