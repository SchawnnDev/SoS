#ifndef __LISTTMP_H
#define __LISTTMP_H

#include "utils.h"
#include "variable.h"

typedef struct{
    char * values[TMP_MAX];
    int numberValues;
} listTmp_t, *ListTmp;

#endif