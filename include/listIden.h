#ifndef __LISTIDEN_H
#define __LISTIDEN_H

#include "utils.h"
#include "variable.h"

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
 * \return Identifier, un pointeur de la liste des identificateurs avec le nouvel élément
*/
Identifier initIdentifier(char* name);

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

#endif