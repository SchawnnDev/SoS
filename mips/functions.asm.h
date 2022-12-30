#ifndef SOS_FUNCTIONS_ASM_H
#define SOS_FUNCTIONS_ASM_H
#include "asm.h"

#define ASM_LOAD_REGISTERS_FROM_STACK_FUNCTION_NAME "_fct_load_registers_from_stack"
#define ASM_SAVE_REGISTERS_TO_STACK_FUNCTION_NAME "_fct_save_registers_to_stack"
#define ASM_BUFFER_LEN_FUNCTION_NAME "_fct_buffer_len"
#define ASM_BUFFER_WRITE_FUNCTION_NAME "_fct_buffer_write"
#define ASM_ATOI_FUNCTION_NAME "_fct_atoi"
#define ASM_STRCMP_FUNCTION_NAME "_fct_strcmp"

/**
 *
 * @return
 */
int asm_writeRegistersToStack();

/**
 *
 * @return
 */
int asm_loadRegistersFromStack();

#endif //SOS_FUNCTIONS_ASM_H
