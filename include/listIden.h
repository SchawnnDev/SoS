#ifndef __LISTIDEN_H
#define __LISTIDEN_H

#include "utils.h"
#include "variable.h"

typedef struct {
    char * name;
    char * value;
    int type;
} identifier_t, *Identifier;

typedef struct {
    Identifier Identifiers[IDEN_MAX];
    int numbreIden; 
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
 * \fn ListIdentifier addIntoListIdentifier(ListIdentifier addr, char* name, char* value)
 * \brief Fonction qui crée un Identifier et l'ajoute en fin de la liste des identificateurs
 * 
 * \param addr : ListIdentifier, la liste des identificateurs
 * \param name : char *, le nom de l'identificateur
 * \param value : char *, la valeur de l'identificateur
 * \param type : int, le type de l'identificateur
 * 
 * \return ListIdentifier, un pointeur de la liste des identificateurs avec le nouvel élément
*/
ListIdentifier addIntoListIdentifier(ListIdentifier addr, char* name, char* value, int type);

#endif