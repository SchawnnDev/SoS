#include "strcmp.asm.h"

int asm_fctStrcmpWritten = FALSE;

/*
# Compares two strings
#
# Arguments:
#   $a0 - address of the first string
#   $a1 - address of the second string
#
# Returns:
#   $v0 - 1 if equal, else 0
 */
int asm_writeStrcmpFunction()
{
    if(asm_fctStrcmpWritten)
        return RETURN_SUCCESS;
    asm_fctStrcmpWritten = TRUE;

    asm_code_printf("\t%s:\n", ASM_STRCMP_FUNCTION_NAME)
    asm_writeRegistersToStack();
    // Code
    asm_code_printf("\t\tmove $t0, $a0\n")
    asm_code_printf("\t\tmove $t1, $a1\n")
    asm_code_printf("\t\tjal %s\n", ASM_BUFFER_LEN_FUNCTION_NAME)
    asm_code_printf("\t\tmove $t2, $v0\n")
    asm_code_printf("\t\tmove $a0, $a1\n")
    asm_code_printf("\t\tjal %s\n", ASM_BUFFER_LEN_FUNCTION_NAME)
    asm_code_printf("\t\tmove $t3, $v0\n")
    asm_code_printf("\t\tbne $t2, $t3, %s_to_false\n", ASM_STRCMP_FUNCTION_NAME)
    asm_code_printf("\t\tmove $a0, $t0\n")
    asm_code_printf("\t\tmove $a1, $t1\n")
    asm_code_printf("\t\t%s_loop:\n", ASM_STRCMP_FUNCTION_NAME)
    asm_code_printf("\t\t\tlb $t0, 0($a0)\n")
    asm_code_printf("\t\t\tlb $t1, 0($a1)\n")
    asm_code_printf("\t\t\tbeq $t0, $zero, %s_to_true\n", ASM_STRCMP_FUNCTION_NAME)
    asm_code_printf("\t\t\tbne $t0, $t1, %s_to_false\n", ASM_STRCMP_FUNCTION_NAME)
    asm_code_printf("\t\t\taddi $a0, $a0, 1\n")
    asm_code_printf("\t\t\taddi $a1, $a1, 1\n")
    asm_code_printf("\t\t\tj %s_loop\n", ASM_STRCMP_FUNCTION_NAME)
    asm_code_printf("\t\t%s_to_true:\n", ASM_STRCMP_FUNCTION_NAME)
    asm_code_printf("\t\t\tli $v0, 1\n")
    asm_code_printf("\t\t\tj %s_end_loop\n", ASM_STRCMP_FUNCTION_NAME)
    asm_code_printf("\t\t%s_to_false:\n", ASM_STRCMP_FUNCTION_NAME)
    asm_code_printf("\t\t\tmove $v0, $zero\n")
    asm_code_printf("\t\t%s_end_loop:\n", ASM_STRCMP_FUNCTION_NAME)
    asm_loadRegistersFromStack();
    asm_code_printf("\t\tjr $ra\n")
    return RETURN_SUCCESS;
}