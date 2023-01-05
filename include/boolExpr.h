#ifndef SOS_BOOLEXPR_H
#define SOS_BOOLEXPR_H

#include <malloc.h>

/*
 -eq : equals
 -ne : not equals
 -gt : strictement supérieur
 -ge : supérieur ou égal
 -lt : strictement inférieur
 -le : inférieur ou égal
 //
 -a : and
 -o : or
 //
 = : string equals
 != : string not equals
 //
 -n : string is empty
 -z : string isn't empty
 */
typedef enum {
    BOOL_EQ,
    BOOL_NEQ,
    BOOL_GT,
    BOOL_GE,
    BOOL_LT,
    BOOL_LE,
    //
    L_AND,
    L_OR,
    //
    STR_EQ,
    STR_NEQ,
    //
    EMPTY,
    NOT_EMPTY
} boolExpr_t;

#endif //SOS_BOOLEXPR_H
