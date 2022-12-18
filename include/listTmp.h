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
 * \return ListIdentifier, un pointeur d'une liste des valeurs temporaires
*/
ListTmp initListTmp();

/*!
 * \fn void cleanListTmp(ListTmp addr)
 * \brief Fonction qui libère la mémoire de la structure des valeurs temporaires
 *
 * \param addr : ListTmp, la liste des valeurs temporaire
*/
void cleanListTmp(ListTmp addr);

/*!
 * \fn ListTmp addIntoListTmp(ListTmp addr, char* value)
 * \brief Fonction qui permet d'ajoute en fin de la liste des valeurs temporaires
 *
 * \param addr : ListTmp, la liste des valeurs temporaires
 * \param value : char *, la valeur à ajouter
 *
 * \return ListTmp, un pointeur de la liste des valeurs temporaires avec le nouvel élément
*/
ListTmp addIntoListTmp(ListTmp addr, char* value);

#endif