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
    L_OR
} boolExpr_t;

#endif //SOS_BOOLEXPR_H
