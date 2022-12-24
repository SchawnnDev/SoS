#ifndef __LISTTMP_H
#define __LISTTMP_H

#include "utils.h"
#include "variable.h"

typedef struct tmpValues_t{
    char * values[TMP_MAX];
    int numberValues;

    struct tmpValues_t* previousTmpValues;
} *TmpValues;

typedef struct{
    TmpValues cursor;
} listTmp_t, *ListTmp;

/*!
 * \fn TmpValues initTmpValues(TmpValues previousTmpValues)
 * \brief Fonction qui initialise la structure des valeurs temporaire
 *
 * \param previousTmpValues : TmpValues, la structure des valeurs temporaire précédante
 *
 * \return ListIdentifier, un pointeur d'une structure des valeurs temporaires
*/
TmpValues initTmpValues(TmpValues previousTmpValues);

/*!
 * \fn void cleanTmpValues(TmpValues addr)
 * \brief Fonction qui libère la mémoire de la structure des valeurs temporaires
 *
 * \param addr : TmpValues, la structure des valeurs temporaire
*/
void cleanTmpValues(TmpValues addr);

/*!
 * \fn ListTmp initListTmp()
 * \brief Fonction qui initialise la liste des valeurs temporaire
 *
 * \return ListIdentifier, un pointeur d'une liste des valeurs temporaires
*/
ListTmp initListTmp();

/*!
 * \fn void cleanListTmp(ListTmp addr)
 * \brief Fonction qui libère la mémoire de la liste des valeurs temporaires
 *
 * \param addr : ListTmp, la liste des valeurs temporaire
*/
void cleanListTmp(ListTmp addr);

/*!
 * \fn int addIntoListTmp(ListTmp addr, char* value)
 * \brief Fonction qui permet d'ajoute en fin de la liste des valeurs temporaires
 *
 * \param addr : ListTmp, la liste des valeurs temporaires
 * \param value : char *, la valeur à ajouter
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int addIntoListTmp(ListTmp addr, char* value);

/*!
 * \fn void addListTmp(ListTmp addr,TmpValues addrTmpValues)
 * \brief Fonction qui permet d'ajoute une structure de valeur temporaire
 *
 * \param addr : ListTmp, la liste des valeurs temporaires
 * \param addrTmpValues : TmpValues, la structure des valeurs temporaires
*/
void addListTmp(ListTmp addr,TmpValues addrTmpValues);

/*!
 * \fn int deleteListTmp(ListTmp addr)
 * \brief Fonction qui permet supprimer la dernière structure de valeur temporaire
 *
 * \param addr : ListTmp, la liste des valeurs temporaires
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int deleteListTmp(ListTmp addr);

#endif