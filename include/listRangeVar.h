#ifndef __LISTRANGEVAR_H
#define __LISTRANGEVAR_H

#include "utils.h"
#include "variable.h"
#include "listIden.h"

typedef struct {
    ListIdentifier listIdentifier;
    int rangeLevel;
} rangeVariable_t, *RangeVariable;

typedef struct {
    RangeVariable nextLevel;
    RangeVariable previousLevel;
} listRangeVariable_t, *ListRangeVariable;

#endif