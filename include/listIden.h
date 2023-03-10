#ifndef __LISTIDEN_H
#define __LISTIDEN_H

#include "utils.h"
#include "variable.h"
#include "memory.h"

typedef struct {
    char * name;
    int type;
    int size;
    MemorySlot memory;
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
 * \fn int addIntoListIdentifier(ListIdentifier addr, char* name, char* value, int offset)
 * \brief Fonction qui crée un Identifier et l'ajoute en fin de la liste des identificateurs
 * 
 * \param addr : ListIdentifier, la liste des identificateurs
 * \param name : char *, le nom de l'identificateur
 * \param offset : int, la valeur de l'offet
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int addIntoListIdentifier(ListIdentifier addr, char* name, MemorySlot offset);

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

/*!
 * \fn int setOffsetOfIdentifier(ListIdentifier addr, int position, int offset)
 * \brief Fonction qui modifie l'offset de l'identificateur
 *
 * \param addr : ListIdentifier, la liste des identificateurs
 * \param position : int, la position de l'identificateur
 * \param offset : int, l'offet par rapport à la stack
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int setOffsetOfIdentifier(ListIdentifier addr, int position, MemorySlot offset);

/*!
 * \fn int getOffsetOfIdentifier(ListIdentifier addr, int position)
 * \brief Fonction qui récupère l'offset de l'identificateur
 *
 * \param addr : ListIdentifier, la liste des identificateurs
 * \param position : int, la position de l'identificateur
 *
 * \return int, l'offet par rapport à la stack
*/
MemorySlot getOffsetOfIdentifier(ListIdentifier addr, int position);

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