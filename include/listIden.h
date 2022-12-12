#ifndef __LISTIDEN_H
#define __LISTIDEN_H

#include "utils.h"
#include "variable.h"

typedef struct {
    char * name;
    char * value;
} identifier_t, *Identifier;

typedef struct {
    Identifier Identifiers[IDEN_MAX];
    int numbreIden; 
} listIdentifier_t, *ListIdentifier;

#endif