#ifndef __LISTRANGEVAR_H
#define __LISTRANGEVAR_H

#include "utils.h"
#include "variable.h"
#include "listIden.h"

typedef struct rangeVariable_t{
    ListIdentifier listIdentifier;
    int rangeLevel;
    int blockType;

    struct rangeVariable_t* nextLevel;
    struct rangeVariable_t* previousLevel;
} *RangeVariable;

typedef struct {
    RangeVariable cursor;
    RangeVariable cursorGlobal;
} listRangeVariable_t, *ListRangeVariable;

typedef struct {
    RangeVariable rangePosition;
    int indexIdentifier;
} variablePosition_t, *VariablePosition;

/*!
 * \fn RangeVariable initRangeVariable(int rangeLevel, RangeVariable previousLevel)
 * \brief Fonction qui initialise la structure de portée de variable
 *
 * \param rangeLevel : int, le niveau de portée de variable
 * \param previousLevel : RangeVariable, le niveau de portée de variable précédant
 *
 * \return RangeVariable, un pointeur d'une structure de portée de variable
*/
RangeVariable
initRangeVariable(int rangeLevel, int blockType, RangeVariable previousLevel);

/*!
 * \fn void cleanRangeVariable(RangeVariable addr)
 * \brief Fonction qui libère la mémoire d'une structure de portée de variable
 *
 * \param addr : RangeVariable, la structure de portée de variable
*/
void cleanRangeVariable(RangeVariable addr);

/*!
 * \fn ListRangeVariable initListRangeVariable()
 * \brief Fonction qui initialise la liste de structure de portée de variable
 *
 * \return ListRangeVariable, un pointeur d'une liste de structure de portée de variable
*/
ListRangeVariable initListRangeVariable();

/*!
 * \fn void cleanListRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui libère la mémoire d'une liste de structure de portée de variable
 *
 * \param addr : ListRangeVariable, la liste de structure de portée de variable
*/
void cleanListRangeVariable(ListRangeVariable addr);

/*!
 * \fn int increaseGlobalRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui augmente la taille de la liste de portée de variable global en ajoutant un nouveau block
 *
 * \param addr : ListRangeVariable, la liste de structure de portée de variable
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int increaseGlobalRangeVariable(ListRangeVariable addr);

/*!
 * \fn int addRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui ajoute un niveau de portée à la liste de structure de portée de variable
 *
 * \param addr : ListRangeVariable, la liste de structure de portée de variable
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int addRangeVariable(ListRangeVariable addr, int blockType);

/*!
 * \fn int deleteRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui supprime un niveau de portée à la liste de structure de portée de variable
 *
 * \param addr : ListRangeVariable, la liste de structure de portée de variable
 *
 *  \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int deleteRangeVariable(ListRangeVariable addr);

/*!
 * \fn VariablePosition initVariablePosition(RangeVariable rangePosition, int indexIdentifier)
 * \brief Fonction qui initialise la structure de position d'identificateur
 *
 * \param rangePosition : RangeVariable, le pointeur du niveau de portée de variable précédant
 * \param previousLevel : VariablePosition, l'index de l'identificateur de la liste d'identificateur de la structure rangePosition
 *
 * \return VariablePosition, un pointeur d'une structure de position
*/
VariablePosition initVariablePosition(RangeVariable rangePosition, int indexIdentifier);

/*!
 * \fn void cleanVariablePosition(VariablePosition addr)
 * \brief Fonction qui libère la mémoire d'une structure de position d'identificateur
 *
 * \param addr : RangeVariable, la structure de position d'identificateur
*/
void cleanVariablePosition(VariablePosition addr);

/*!
 * \fn VariablePosition searchIdentifierPosition(ListRangeVariable addr, char* name)
 * \brief Fonction qui cherche la position de l'identificateur dans la liste des postée de variable
 *
 * \param addr : ListRangeVariable, la liste de structure de portée de variable
 * \param name : char*, le nom de l'identificateur
 *
 * \return VariablePosition, un pointeur d'une structure de position
*/
VariablePosition searchIdentifierPosition(ListRangeVariable addr, char* name);

/*!
 * \fn int addIdentifier(ListRangeVariable addr, char* name)
 * \brief Fonction qui ajoute un identificateur dans la liste des postée de variable
 *
 * \param addr : ListRangeVariable, la liste de structure de portée de variable
 * \param name : char*, le nom de l'identificateur
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int addIdentifier(ListRangeVariable addr, char *name);

/*!
 * \fn int addLocalIdentifier(ListRangeVariable addr, char* name)
 * \brief Fonction qui ajoute un identificateur dans la liste des postée de variable
 *
 * \param addr : ListRangeVariable, la liste de structure de portée de variable
 * \param name : char*, le nom de l'identificateur
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int addLocalIdentifier(ListRangeVariable addr, char *name);

/*!
 * \fn int setType(ListRangeVariable addr, char* name, int type)
 * \brief Fonction qui modifie le type de l'identificateur dans la liste des postée de variable
 *
 * \param addr : ListRangeVariable, la liste de structure de portée de variable
 * \param name : char*, le nom de l'identificateur
 * \param type : int, le type de l'identificateur
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int setType(ListRangeVariable addr, char* name, int type);

/*!
 * \fn int setArraySize(ListRangeVariable addr, char* name, int arraySize)
 * \brief Fonction qui modifie le type de l'identificateur dans la liste des postée de variable
 *
 * \param addr : ListRangeVariable, la liste de structure de portée de variable
 * \param name : char*, le nom de l'identificateur
 * \param ListTmp : addrTmp, la liste des valeurs temporaire
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int setArraySize(ListRangeVariable addr, char* name, int arraySize);

/*!
 * \fn int printIdentifierFromListRange(ListRangeVariable addr,char* name)
 * \brief Fonction affiche l'état actuelle d'un identificateur
 *
 * \param addr : ListRangeVariable, la liste de structure de portée de variable
 * \param position : int, la position de l'identificateur dans la liste des identificateurs
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int printIdentifierFromListRange(ListRangeVariable addr,char* name);

#endif