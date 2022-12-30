#include "bufferWrite.asm.h"

int asm_fctBufferWriteWritten = FALSE;

/*
# Function to write a buffer to the heap
#
# Arguments:
#   $a0: Pointer to the start of the buffer
#   $a1: Number of elements in the buffer
#
# Returns:
#   $v0: Pointer to the start of the allocated memory on the heap
 */
int asm_writeBufferWriteFunction()
{
    if(asm_fctBufferWriteWritten)
        return RETURN_SUCCESS;
    asm_fctBufferWriteWritten = TRUE;
    asm_code_printf("\t%s:\n", ASM_BUFFER_WRITE_FUNCTION_NAME)
    asm_writeRegistersToStack();
    // Code
    asm_code_printf("\t\tmove $t0, $a0\n")
    asm_code_printf("\t\tli $v0, 9\n")
    asm_code_printf("\t\tli $a0, 4\n")
    asm_code_printf("\t\tmul $a0, $a1, $a0\n")
    asm_code_printf("\t\tsyscall\n")
    asm_code_printf("\t\tmove $t1, $v0\n")
    asm_code_printf("\t\tmove $t4, $v0\n")
    asm_code_printf("\t\taddi $t2, $zero, 0\n")
    asm_code_printf("\t\t%s_loop:\n",ASM_BUFFER_WRITE_FUNCTION_NAME)
    asm_code_printf("\t\t\tlw $t3, 0($t0)\n")
    asm_code_printf("\t\t\tsw $t3, 0($t1)\n")
    asm_code_printf("\t\t\taddi $t1, $t1, 4\n")
    asm_code_printf("\t\t\taddi $t2, $t2, 1\n")
    asm_code_printf("\t\t\taddi $t0, $t0, 4\n")
    asm_code_printf("\t\t\tbne $t2, $a1, %s_loop\n",ASM_BUFFER_WRITE_FUNCTION_NAME)
    asm_code_printf("\t\tmove $v0, $t4\n")
    asm_loadRegistersFromStack();
    asm_code_printf("\t\tjr $ra\n")
    return RETURN_SUCCESS;
}
