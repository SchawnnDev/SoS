#include "header.asm.h"

int asm_writeHeader_written = FALSE;

int asm_writeHeader()
{
    if (asm_writeHeader_written) return RETURN_FAILURE;
    asm_writeHeader_written = TRUE;

    asm_data_printf("\t%s: .word %d\n", ASM_VAR_GLOBAL_READ_BUFFER_LEN_NAME, ASM_VAR_GLOBAL_READ_BUFFER_SIZE)
    asm_data_printf("\t%s: .space %d\n", ASM_VAR_GLOBAL_READ_BUFFER_NAME, ASM_VAR_GLOBAL_READ_BUFFER_SIZE)
    asm_data_printf("\t%s: .word 0\n", ASM_VAR_OFFSET_NAME)
    asm_data_printf("\t%s: .word 0\n", ASM_VAR_ARGC)
    asm_data_printf("\t%s: .word 0\n", ASM_VAR_ARGV_START)
    asm_data_printf("\t%s: .asciiz \"Nan\"\n", ASM_VAR_ERROR_NAN)
    asm_data_printf("\t%s: .asciiz \"Index out of bounds\"\n", ASM_VAR_ERROR_OUTOFBOUNDS)
    asm_data_printf("\t%s: .asciiz \"Segfault: Array element not allocated\"\n", ASM_VAR_ERROR_ARRAY_ELEMENT_NOT_ALLOCATED)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}
