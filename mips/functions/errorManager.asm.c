#include "errorManager.asm.h"

int asm_writeErrorManagerFunctions()
{
    asm_writePrintErrorAndExit();
    asm_writeOutOfBoundsErrorFunction();
    asm_writeNotANumberErrorFunction();
    asm_writeArrayElementNotAllocatedErrorFunction();
    asm_writeNonExistentArgumentErrorFunction();
    return RETURN_SUCCESS;
}

int asm_writePrintErrorAndExit(){
    asm_code_printf("\t%s:\n", ASM_PRINT_AND_EXIT_ERROR_FUNCTION_NAME)

    asm_code_printf("\t\tli $v0, 4\n")
    asm_code_printf("\t\tsyscall\n")
    asm_code_printf("\t\tmove $a0, $a1\n")
    asm_code_printf("\t\tli $v0, 17\n")
    asm_code_printf("\t\tsyscall\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_fctOutOfBoundsErrorWritten = FALSE;

int asm_writeOutOfBoundsErrorFunction(){
    if(asm_fctOutOfBoundsErrorWritten)
        return RETURN_SUCCESS;
    asm_fctOutOfBoundsErrorWritten = TRUE;

    asm_code_printf("\t%s:\n", ASM_OUT_OF_BOUNDS_ERROR_FUNCTION_NAME)
    asm_code_printf("\t\tla $a0, %s\n", ASM_VAR_ERROR_OUTOFBOUNDS)
    // Error code for outofbounds : 1
    asm_code_printf("\t\tli $a1, 1\n")
    asm_code_printf("\t\tj %s\n", ASM_PRINT_AND_EXIT_ERROR_FUNCTION_NAME)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_fctNotANumberErrorWritten = FALSE;

int asm_writeNotANumberErrorFunction(){
    if(asm_fctNotANumberErrorWritten)
        return RETURN_SUCCESS;
    asm_fctNotANumberErrorWritten = TRUE;

    asm_code_printf("\t%s:\n", ASM_NOT_A_NUMBER_ERROR_FUNCTION_NAME)
    asm_code_printf("\t\tla $a0, %s\n", ASM_VAR_ERROR_NAN)
    // Error code for nan : 2
    asm_code_printf("\t\tli $a1, 2\n")
    asm_code_printf("\t\tj %s\n", ASM_PRINT_AND_EXIT_ERROR_FUNCTION_NAME)
    
    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_fctArrayElementNotAllocatedErrorWritten = FALSE;

int asm_writeArrayElementNotAllocatedErrorFunction(){
    if(asm_fctArrayElementNotAllocatedErrorWritten)
        return RETURN_SUCCESS;
    asm_fctArrayElementNotAllocatedErrorWritten = TRUE;

    asm_code_printf("\t%s:\n", ASM_ARRAY_ELEMENT_NOT_ALLOCATED_ERROR_FUNCTION_NAME)
    asm_code_printf("\t\tla $a0, %s\n", ASM_VAR_ERROR_ARRAY_ELEMENT_NOT_ALLOCATED)
    // Error code for array element not allocated : 3
    asm_code_printf("\t\tli $a1, 3\n")
    asm_code_printf("\t\tj %s\n", ASM_PRINT_AND_EXIT_ERROR_FUNCTION_NAME)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

int asm_fctNonExistentArgumentErrorWritten = FALSE;

int asm_writeNonExistentArgumentErrorFunction(){
    if(asm_fctNonExistentArgumentErrorWritten)
        return RETURN_SUCCESS;
    asm_fctNonExistentArgumentErrorWritten = TRUE;

    asm_code_printf("\t%s:\n", ASM_NON_EXISTENT_ARGUMENT_ERROR_FUNCTION_NAME)
    asm_code_printf("\t\tla $a0, %s\n", ASM_VAR_ERROR_NON_EXISTENT_ARGUMENT)
    // Error code for argument non existent : 4
    asm_code_printf("\t\tli $a1, 3\n")
    asm_code_printf("\t\tj %s\n", ASM_PRINT_AND_EXIT_ERROR_FUNCTION_NAME)

    return RETURN_SUCCESS;
}