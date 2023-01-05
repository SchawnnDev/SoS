#include "intToString.asm.h"

int asm_fctIntToStringWritten = FALSE;

/*
# Convert an integer to a string
#
# Arguments:
#   $a0 - address of the integer
#
# Returns:
#   $v0 - the string representation of the integer
 */
int asm_writeIntToStringFunction()
{
    if(asm_fctIntToStringWritten)
        return RETURN_SUCCESS;
    asm_fctIntToStringWritten = TRUE;

    asm_code_printf("\t%s:\n", ASM_INT_TO_STRING_FUNCTION_NAME)
    asm_writeRegistersToStack();

    // Code
    asm_code_printf("\t\tlw $s0, 0($a0)\n");
    asm_code_printf("\t\tlw $s1, 0($a0)\n");
    asm_code_printf("\t\taddi $t0, $zero, 10\n");
    asm_code_printf("\t\taddi $t1, $zero, 1\n");
    asm_code_printf("\t\tslt $t3, $s1, $zero\n");
    asm_code_printf("\t\t%s_count:\n", ASM_INT_TO_STRING_FUNCTION_NAME)
    asm_code_printf("\t\tdiv $s0, $t0\n");
    asm_code_printf("\t\tmflo $t2\n");
    asm_code_printf("\t\tbeqz $t2, %s_allocate\n", ASM_INT_TO_STRING_FUNCTION_NAME);
    asm_code_printf("\t\tmove $s0, $t2\n");
    asm_code_printf("\t\taddi $t1, $t1, 1\n");
    asm_code_printf("\t\tj %s_count\n", ASM_INT_TO_STRING_FUNCTION_NAME);
    asm_code_printf("\t\t%s_allocate:\n", ASM_INT_TO_STRING_FUNCTION_NAME);
    asm_code_printf("\t\tmove $a0, $t1\n");
    asm_code_printf("\t\taddi $a0, $a0, 1\n");
    asm_code_printf("\t\tadd $a0, $a0, $t3\n");
    asm_code_printf("\t\tli $v0, 9\n");
    asm_code_printf("\t\tsyscall\n");
    asm_code_printf("\t\tmove $s0, $s1\n");
    asm_code_printf("\t\tmove $s2, $v0\n");
    asm_code_printf("\t\tadd $s2, $s2, $t1\n");
    asm_code_printf("\t\tadd $s2, $s2, $t3\n");
    asm_code_printf("\t\tsb $zero, 0($s2)\n");
    asm_code_printf("\t\taddi $s2, $s2, -1\n");
    asm_code_printf("\t\t%s_writing_heap:\n", ASM_INT_TO_STRING_FUNCTION_NAME);
    asm_code_printf("\t\tabs $s0, $s0\n");
    asm_code_printf("\t\tdivu $s0, $t0\n");
    asm_code_printf("\t\tmfhi $t4\n");
    asm_code_printf("\t\taddi $t4, $t4, '0'\n");
    asm_code_printf("\t\tsb $t4, 0($s2) \n");
    asm_code_printf("\t\tmflo $s0\n");
    asm_code_printf("\t\taddi $t1, $t1, -1\n");
    asm_code_printf("\t\taddi $s2, $s2, -1\n");
    asm_code_printf("\t\tbgtz $t1, %s_writing_heap\n", ASM_INT_TO_STRING_FUNCTION_NAME);
    asm_code_printf("\t\tbeqz $t3, %s_end\n", ASM_INT_TO_STRING_FUNCTION_NAME);
    asm_code_printf("\t\tli $t5, '-'\n");
    asm_code_printf("\t\tsb $t5, 0($s2)\n");
    asm_code_printf("\t\t%s_end:\n", ASM_INT_TO_STRING_FUNCTION_NAME);
    asm_code_printf("\t\tjal fct_load_temp_values\n");
    asm_code_printf("\t\tlw $ra, 0($sp)\n");
    asm_code_printf("\t\taddi $sp, $sp, 4\n");

    asm_loadRegistersFromStack();
    asm_code_printf("\t\tjr $ra\n")
    return RETURN_SUCCESS;








}
