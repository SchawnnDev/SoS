#include "bufferWrite.asm.h"

int asm_fctBufferWriteWritten = FALSE;

/*
# Function to write a buffer to the heap until '\0' char (not included)
#
# Arguments:
#   $a0: Source Pointer to the start of the buffer
#   $a1: Destination Pointer to the start of the buffer
#
# Returns:
#   $v0: Pointer of the actual position on the heap
 */
int asm_writeBufferWriteFunction()
{
    if(asm_fctBufferWriteWritten)
        return RETURN_SUCCESS;
    asm_fctBufferWriteWritten = TRUE;

    asm_code_printf("\t%s:\n", ASM_BUFFER_WRITE_FUNCTION_NAME)
    asm_writeRegistersToStack();
    // Code
    // Ignore sbrk
//    asm_code_printf("\t\tli $v0, 9\n")
//    asm_code_printf("\t\tli $a0, 4\n")
//    asm_code_printf("\t\tmul $a0, $a1, $a0\n")
//    asm_code_printf("\t\tsyscall\n")
    asm_code_printf("\t\tmove $t0, $a0\n")
    asm_code_printf("\t\tmove $t1, $a1\n")
    asm_code_printf("\t\t%s_loop:\n",ASM_BUFFER_WRITE_FUNCTION_NAME)
    asm_code_printf("\t\t\tlb $t3, 0($t0)\n")
    asm_code_printf("\t\t\tbeq $t3, $zero, %s_exit\n", ASM_BUFFER_WRITE_FUNCTION_NAME)
    asm_code_printf("\t\t\tsb $t3, 0($t1)\n")
    asm_code_printf("\t\t\taddi $t1, $t1, 1\n")
    asm_code_printf("\t\t\taddi $t0, $t0, 1\n")
    asm_code_printf("\t\t\tb %s_loop\n",ASM_BUFFER_WRITE_FUNCTION_NAME)
    asm_code_printf("\t\t%s_exit:\n", ASM_BUFFER_WRITE_FUNCTION_NAME)
    asm_code_printf("\t\t\tmove $v0, $t1\n")
    asm_loadRegistersFromStack();
    asm_code_printf("\t\t\tjr $ra\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}
