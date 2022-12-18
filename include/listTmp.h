#ifndef __LISTTMP_H
#define __LISTTMP_H

#include "utils.h"
#include "variable.h"

typedef struct{
    char * values[TMP_MAX];
    int numberValues;
} listTmp_t, *ListTmp;

/*!
 * \fn ListTmp initListTmp()
 * \brief Fonction qui initialise la structure des valeurs temporaire
 *
 * \return ListIdentifier, un pointeur d'une liste des valeurs temporaire
*/
ListTmp initListTmp();

#endif