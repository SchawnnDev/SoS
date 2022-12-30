#ifndef __VAR_H
#define __VAR_H

/* Bool */
#define FALSE 0
#define TRUE 1

/* Tailles */
#define IDEN_MAX 100
#define TMP_TAB_MAX 100
#define TEXT_TAB_MAX 100
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

/* Operations */
#define PLUS_OPE 1
#define MINUS_OPE 2
#define MULT_OPE 3
#define DIV_OPE 4
#define MOD_OPE 5
#define SIZE_INT_STR 12

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

#endif