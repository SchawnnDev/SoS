#ifndef __LISTTMP_H
#define __LISTTMP_H

#include "utils.h"
#include "variable.h"

typedef struct{
    char * valeurs[TMP_MAX];
    int nombreValeurs;
} listTmp_t, *ListTmp;

#endif