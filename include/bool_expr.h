#ifndef SOS_BOOL_EXPR_H
#define SOS_BOOL_EXPR_H

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

typedef struct s_boolExpr *boolExpr;

struct s_boolExpr {
    // Type is != AND != OR
    const char *reg_a;
    const char *reg_b;
    // Type is == AND == OR
    boolExpr expr_a;
    boolExpr expr_b;
    //
    const boolExpr_t type;
    //
    const char* reg_to;
} ;

boolExpr initBoolExpr() {
    return malloc(sizeof(struct s_boolExpr));
}

int computeBoolExpr(boolExpr expr) {

    switch (expr->type) {
        case BOOL_EQ:
            break;
        case BOOL_NEQ:
            break;
        case BOOL_GT:
            break;
        case BOOL_GE:
            break;
        case BOOL_LT:
            break;
        case BOOL_LE:
            break;
        case L_AND:
            break;
        case L_OR:
            break;
    }

    return 0;
}


// (a && b) || (!c && d)
// Or{And{a, b}, And{Not{c}, d}}


#endif //SOS_BOOL_EXPR_H
