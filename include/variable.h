#ifndef __VAR_H
#define __VAR_H

/* Tailles */
#define IDEN_MAX 100
#define TMP_TAB_MAX 100
#define TEXT_TAB_MAX 100
#define DATA_TAB_MAX 100
#define CODE_TAB_MAX 100
#define ASM_PRINTF_BUF_MAX 256

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

// Section
#define SECTION_DATA 1 << 0
#define SECTION_TEXT 1 << 1

#endif