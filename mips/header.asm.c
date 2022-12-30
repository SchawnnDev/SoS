#include "asm.h"
#include "header.asm.h"

int asm_writeHeader_written = FALSE;

int asm_writeHeader()
{
    if (asm_writeHeader_written) return RETURN_FAILURE;
    asm_writeHeader_written = TRUE;
    asm_data_printf("\t%s: .word %d\n", ASM_VAR_GLOBAL_READ_BUFFER_LEN_NAME, ASM_VAR_GLOBAL_READ_BUFFER_SIZE)
    asm_data_printf("\t%s: .space %d\n", ASM_VAR_GLOBAL_READ_BUFFER_NAME, ASM_VAR_GLOBAL_READ_BUFFER_SIZE)
    asm_data_printf("\t%s: .word 0\n", ASM_VAR_OFFSET_NAME)
    return RETURN_SUCCESS;
}
