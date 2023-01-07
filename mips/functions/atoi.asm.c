#include "atoi.asm.h"

int asm_fctAtoiWritten = FALSE;

/*
# Convert a string to an integer
#
# Arguments:
#   $a0 - address of the string
#
# Returns:
#   $v0 - the integer representation of the string
 */
int asm_writeAtoiFunction()
{
    if(asm_fctAtoiWritten)
        return RETURN_SUCCESS;
    asm_fctAtoiWritten = TRUE;

    asm_code_printf("\t%s:\n", ASM_ATOI_FUNCTION_NAME)
    asm_writeRegistersToStack();
    // Code
    asm_code_printf("\t\tmove $a1, $a0\n")
    asm_code_printf("\t\tjal %s\n", ASM_BUFFER_LEN_FUNCTION_NAME)
    asm_code_printf("\t\tbeq $v0, $zero, %s\n", ASM_NOT_A_NUMBER_ERROR_FUNCTION_NAME)
    asm_code_printf("\t\tmove $a0, $a1\n")
    asm_code_printf("\t\tlb $t0, 0($a0)\n")
    asm_code_printf("\t\tli $t5, 0\n")
    asm_code_printf("\t\tli $t6, 1\n")
    asm_code_printf("\t\tli $t7, '+'\n")
    asm_code_printf("\t\tbltu $t0, $t7, %s\n", ASM_NOT_A_NUMBER_ERROR_FUNCTION_NAME)
    asm_code_printf("\t\tli $t7, '9'\n")
    asm_code_printf("\t\tbltu $t7, $t0, %s\n", ASM_NOT_A_NUMBER_ERROR_FUNCTION_NAME)
    asm_code_printf("\t\tli $t7, ','\n")
    asm_code_printf("\t\tbeq $t7, $t0, %s\n", ASM_NOT_A_NUMBER_ERROR_FUNCTION_NAME)
    asm_code_printf("\t\tli $t7, '.'\n")
    asm_code_printf("\t\tbeq $t7, $t0, %s\n", ASM_NOT_A_NUMBER_ERROR_FUNCTION_NAME)
    asm_code_printf("\t\tli $t7, '/'\n")
    asm_code_printf("\t\tbeq $t7, $t0, %s\n", ASM_NOT_A_NUMBER_ERROR_FUNCTION_NAME)
    asm_code_printf("\t\tli $t7, '+'\n")
    asm_code_printf("\t\tbeq $t7, $t0, %s_set_plus\n", ASM_ATOI_FUNCTION_NAME)
    asm_code_printf("\t\tli $t7, '-'\n")
    asm_code_printf("\t\tbeq $t7, $t0, %s_set_minus\n", ASM_ATOI_FUNCTION_NAME)
    asm_code_printf("\t\t%s_init:\n", ASM_ATOI_FUNCTION_NAME)
    asm_code_printf("\t\t\tli $t2, 10\n")
    asm_code_printf("\t\t\tli $t1, 1\n")
    asm_code_printf("\t\t\tmove $t7, $v0\n")
    asm_code_printf("\t\t\tmove $v0, $zero\n")
    asm_code_printf("\t\t\tbeq $t7, 1, %s_loop\n", ASM_ATOI_FUNCTION_NAME)
    asm_code_printf("\t\t%s_setup:\n", ASM_ATOI_FUNCTION_NAME)
    asm_code_printf("\t\t\tmul $t1, $t1, 10\n")
    asm_code_printf("\t\t\taddi $t6, $t6, 1\n")
    asm_code_printf("\t\t\tblt $t6, $t7, %s_setup\n", ASM_ATOI_FUNCTION_NAME)
    asm_code_printf("\t\t%s_loop:\n", ASM_ATOI_FUNCTION_NAME)
    asm_code_printf("\t\t\tlb $t0, 0($a0)\n")
    asm_code_printf("\t\t\tbeq $t0, $zero, %s_end_loop\n", ASM_ATOI_FUNCTION_NAME)
    asm_code_printf("\t\t\tli $t7, '0'\n")
    asm_code_printf("\t\t\tbltu $t0, $t7, %s\n", ASM_NOT_A_NUMBER_ERROR_FUNCTION_NAME)
    asm_code_printf("\t\t\tli $t7, '9'\n")
    asm_code_printf("\t\t\tbltu $t7, $t0, %s\n", ASM_NOT_A_NUMBER_ERROR_FUNCTION_NAME)
    asm_code_printf("\t\t\tsub $t0, $t0, '0'\n")
    asm_code_printf("\t\t\tmul $t0, $t0, $t1\n")
    asm_code_printf("\t\t\tadd $v0, $v0, $t0\n")
    asm_code_printf("\t\t\tdiv $t1, $t2\n")
    asm_code_printf("\t\t\tmflo $t1\n")
    asm_code_printf("\t\t\taddi $a0, $a0, 1\n")
    asm_code_printf("\t\t\tj %s_loop\n", ASM_ATOI_FUNCTION_NAME)
    asm_code_printf("\t\t%s_set_minus:\n", ASM_ATOI_FUNCTION_NAME)
    asm_code_printf("\t\t\tli $t5, 1\n")
    asm_code_printf("\t\t%s_set_plus:\n", ASM_ATOI_FUNCTION_NAME)
    asm_code_printf("\t\t\taddi $t6, $t6, 1\n")
    asm_code_printf("\t\t\taddi $a0, $a0, 1\n")
    asm_code_printf("\t\t\tli $t3, 3\n")
    asm_code_printf("\t\t\tbgt $v0, $t3, %s_init\n", ASM_ATOI_FUNCTION_NAME)
    asm_code_printf("\t\t\tsubi $v0, $v0, 1\n")
    asm_code_printf("\t\t\tj %s_init\n", ASM_ATOI_FUNCTION_NAME)
    asm_code_printf("\t\t%s_end_loop:\n", ASM_ATOI_FUNCTION_NAME)
    asm_code_printf("\t\t\tbne $t5, 1, %s_end\n", ASM_ATOI_FUNCTION_NAME)
    asm_code_printf("\t\t%s_apply_minus:\n", ASM_ATOI_FUNCTION_NAME)
    asm_code_printf("\t\t\tmul $v0, $v0, -1\n")
    asm_code_printf("\t\t%s_end:\n", ASM_ATOI_FUNCTION_NAME)
    asm_loadRegistersFromStack();
    asm_code_printf("\t\tjr $ra\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}