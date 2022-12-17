#ifndef __LISTIDEN_H
#define __LISTIDEN_H

#include "utils.h"
#include "variable.h"
#include "listTmp.h"

typedef struct {
    char * name;
    char ** values;
    int type;
    int arraySize;
} identifier_t, *Identifier;

typedef struct {
    Identifier Identifiers[IDEN_MAX];
    int numberIdentifiers;
} listIdentifier_t, *ListIdentifier;

/*!
 * \fn ListIdentifier initListIdentifier()
 * \brief Fonction qui initialise la structure des Identificateurs
 * 
 * \return ListIdentifier, un pointeur d'une liste des identificateurs
*/
ListIdentifier initListIdentifier();

/*!
 * \fn void cleanListIdentifier(ListIdentifier addr)
 * \brief Fonction qui libère la mémoire de la structure des Identificateurs
 * 
 * \param addr : ListIdentifier, la liste des identificateurs
*/
void cleanListIdentifier(ListIdentifier addr);

/*!
 * \fn Identifier initIdentifier(char* name)
 * \brief Fonction qui crée un Identifier
 *
 * \param name : char *, le nom de l'identificateur
 *
 * \return Identifier, un pointeur d'identificateur avec le nom passé en paramètre.
*/
Identifier initIdentifier(char* name);

/*!
 * \fn int searchIdentifierPosition(ListIdentifier addr, char* name)
 * \brief Fonction qui recherche la position de l'Identifier dans la liste des identificateurs
 *
 * \param addr : ListIdentifier, la liste des identificateurs
 * \param name : char *, le nom de l'identificateur
 *
 * \return int, la position de l'identificateur (NOTFOUND lorsqu'il n'est pas présent)
*/
int searchIdentifierPosition(ListIdentifier addr, char* name);

/*!
 * \fn ListIdentifier addIntoListIdentifier(ListIdentifier addr, char* name, char* value)
 * \brief Fonction qui crée un Identifier et l'ajoute en fin de la liste des identificateurs
 * 
 * \param addr : ListIdentifier, la liste des identificateurs
 * \param name : char *, le nom de l'identificateur
 *
 * \return ListIdentifier, un pointeur de la liste des identificateurs avec le nouvel élément
*/
ListIdentifier addIntoListIdentifier(ListIdentifier addr, char* name);

/*!
 * \fn ListIdentifier setTypeOfIdentifier(ListIdentifier addr, char* name,int type)
 * \brief Fonction qui modifie le type de l'identificateur
 *
 * \param addr : ListIdentifier, la liste des identificateurs
 * \param name : char *, le nom de l'identificateur
 * \param type : int, le type de l'identificateur
 *
 * \return ListIdentifier, un pointeur de la liste des identificateurs avec le nouveau type
*/
ListIdentifier setTypeOfIdentifier(ListIdentifier addr, char* name,int type);

/*!
 * \fn ListIdentifier setValuesOfIdentifierFromListTmp(ListIdentifier addr, char* name,ListTmp addrTmp)
 * \brief Fonction remplie le tableau des valeurs de l'identificateur grave à la liste temporaire
 *
 * \param addr : ListIdentifier, la liste des identificateurs
 * \param name : char *, le nom de l'identificateur
 * \param ListTmp : addrTmp, la liste des valeurs temporaire
 *
 * \return ListIdentifier, un pointeur de la liste des identificateurs avec les valeurs
*/
ListIdentifier setValuesOfIdentifierFromListTmp(ListIdentifier addr, char* name,ListTmp addrTmp);

#endif