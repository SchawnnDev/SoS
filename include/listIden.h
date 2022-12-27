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
    int offset;
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
 * \fn void cleanIdentifier(Identifier addr)
 * \brief Fonction qui libère la mémoire de la structure Identificateur
 *
 * \param addr : Identifier, un identificateur
*/
void cleanIdentifier(Identifier addr);

/*!
 * \fn int searchIntoListIdentifiern(ListIdentifier addr, char* name)
 * \brief Fonction qui recherche la position de l'Identifier dans la liste des identificateurs
 *
 * \param addr : ListIdentifier, la liste des identificateurs
 * \param name : char *, le nom de l'identificateur
 *
 * \return int, la position de l'identificateur (NOTFOUND lorsqu'il n'est pas présent)
*/
int searchIntoListIdentifier(ListIdentifier addr, char* name);

/*!
 * \fn int addIntoListIdentifier(ListIdentifier addr, char* name, char* value)
 * \brief Fonction qui crée un Identifier et l'ajoute en fin de la liste des identificateurs
 * 
 * \param addr : ListIdentifier, la liste des identificateurs
 * \param name : char *, le nom de l'identificateur
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int addIntoListIdentifier(ListIdentifier addr, char* name);

/*!
 * \fn int setTypeOfIdentifier(ListIdentifier addr, int position,int type)
 * \brief Fonction qui modifie le type de l'identificateur
 *
 * \param addr : ListIdentifier, la liste des identificateurs
 * \param position : int, la position de l'identificateur
 * \param type : int, le type de l'identificateur
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int setTypeOfIdentifier(ListIdentifier addr, int position,int type);

/*!
 * \fn int setValuesOfIdentifierFromListTmp(ListIdentifier addr, int position, ListTmp addrTmp)
 * \brief Fonction remplie le tableau des valeurs de l'identificateur depuis à la liste temporaire
 *
 * \param addr : ListIdentifier, la liste des identificateurs
 * \param position : int, la position de l'identificateur
 * \param addrTmp : ListTmp, la liste des valeurs temporaire
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int setValuesOfIdentifierFromListTmp(ListIdentifier addr, int position, ListTmp addrTmp);

/*!
 * \fn int getValuesFromIdentifierToListTmp(ListIdentifier addr, int position, int index, ListTmp addrTmp)
 * \brief Fonction remplie liste temporaire depuis le tableau des valeurs de l'identificateur
 *
 * \param addr : ListIdentifier, la liste des identificateurs
 * \param position : int, la position de l'identificateur
 * \param index : int, la position de la données qu'on souhaite récupérer (-1 => toutes les données)
 * \param addrTmp : out : ListTmp, la liste des valeurs temporaire
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int getValuesFromIdentifierToListTmp(ListIdentifier addr, int position, int index, ListTmp addrTmp);

/*!
 * \fn int setTypeOfIdentifier(ListIdentifier addr, int position, int type)
 * \brief Fonction qui modifie le type de l'identificateur
 *
 * \param addr : ListIdentifier, la liste des identificateurs
 * \param position : int, la position de l'identificateur
 * \param ListTmp : addrTmp, la liste des valeurs temporaire
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int setArraySizeOfIdentifier(ListIdentifier addr, int position, int arraySize);

int setRegisterOfIdentifier(ListIdentifier addr, int position, int offset);
int getRegisterOfIdentifier(ListIdentifier addr, int position, ListTmp addrTmp);

/*!
 * \fn int printIdentifier(ListIdentifier addr,int position)
 * \brief Fonction affiche l'état actuelle d'un identificateur
 *
 * \param addr : ListIdentifier, la liste des identificateurs
 * \param position : int, la position de l'identificateur dans la liste des identificateurs
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int printIdentifier(ListIdentifier addr,int position);

/*!
 * \fn void printListIdentifier(ListIdentifier addr)
 * \brief Fonction affiche l'état actuelle de la liste des identificateurs
 *
 * \param addr : ListIdentifier, la liste des identificateurs
*/
void printListIdentifier(ListIdentifier addr);

#endif