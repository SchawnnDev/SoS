#ifndef VAR_H
#define VAR_H

/* Bool */
#define FALSE 0
#define TRUE 1

/* Tailles */
#define IDEN_MAX 100
#define DATA_TAB_MAX 100
#define CODE_TAB_MAX 100
#define ASM_PRINTF_BUF_MAX 256
#define ASM_INTEGER_SIZE 4

/* Error */
#define NOTFOUND -1
#define RETURN_FAILURE -1
#define RETURN_SUCCESS 0

/* Types */
#define UNSET 0
#define CHAIN 1
#define INTEGER 2
#define ARRAY 3
#define FUNCTION 4
#define MAXTYPEVALUE 5 /* /!\  A modifier lors de l'ajout ou suppression d'un type */

/* Blocks */
#define BLOCK_MAIN 0
#define BLOCK_IF 1
#define BLOCK_WHILE_UNTIL 2
#define BLOCK_FOR 3
#define BLOCK_FUNCTION 4
#define BLOCK_CASE 5

/* Operations */
#define PLUS_OPE 1
#define MINUS_OPE 2
#define MULT_OPE 3
#define DIV_OPE 4
#define MOD_OPE 5

/* ASM Generation */
#define START_STACK 32
#define ADDR_STACK_SIZE 4
#define TYPE_STACK 0
#define TYPE_DATA 1

/* ASM MIPS Functions */
#define ASM_LOAD_REGISTERS_FROM_STACK_FUNCTION_NAME "_fct_load_registers_from_stack"
#define ASM_SAVE_REGISTERS_TO_STACK_FUNCTION_NAME "_fct_save_registers_to_stack"
#define ASM_BUFFER_LEN_FUNCTION_NAME "_fct_buffer_len"
#define ASM_BUFFER_WRITE_FUNCTION_NAME "_fct_buffer_write"
#define ASM_ATOI_FUNCTION_NAME "_fct_atoi"
#define ASM_STRCMP_FUNCTION_NAME "_fct_strcmp"
#define ASM_DISPLAY_STRING_FUNCTION_NAME "_fct_display_string"
#define ASM_INT_TO_STRING_FUNCTION_NAME "_fct_int_to_string"
#define ASM_OUT_OF_BOUNDS_ERROR_FUNCTION_NAME "_fct_out_of_bounds_error"
#define ASM_NOT_A_NUMBER_ERROR_FUNCTION_NAME "_fct_not_a_number_error"
#define ASM_ARRAY_ELEMENT_NOT_ALLOCATED_ERROR_FUNCTION_NAME "_fct_array_element_not_allocated_error"
#define ASM_NON_EXISTENT_ARGUMENT_ERROR_FUNCTION_NAME "_fct_non_existent_argument_error"
#define ASM_PRINT_AND_EXIT_ERROR_FUNCTION_NAME "_fct_print_and_exit_error"

/* ASM MIPS Variables */
#define ASM_VAR_OFFSET_NAME "_offset"
#define ASM_VAR_GLOBAL_READ_BUFFER_LEN_NAME "_global_read_buffer_len"
#define ASM_VAR_GLOBAL_READ_BUFFER_NAME "_global_read_buffer"
#define ASM_VAR_GLOBAL_READ_BUFFER_SIZE 255 // -1 fort NUL char
#define ASM_VAR_ERROR_NAN "_error_nan"
#define ASM_VAR_ERROR_OUTOFBOUNDS "_error_outofbounds"
#define ASM_VAR_ERROR_ARRAY_ELEMENT_NOT_ALLOCATED "_error_array_element_not_allocated"
#define ASM_VAR_ERROR_NON_EXISTENT_ARGUMENT "_error_not_existent_argument"
#define ASM_VAR_ARGC "_argc"
#define ASM_VAR_ARGV_START "_argv_start"
#define ASM_VAR_REGISTERS_CACHE_COUNT 18
#define ASM_VAR_REGISTERS_CACHE_SIZE ASM_VAR_REGISTERS_CACHE_COUNT * ASM_INTEGER_SIZE

#endif