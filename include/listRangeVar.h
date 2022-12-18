#ifndef __LISTRANGEVAR_H
#define __LISTRANGEVAR_H

#include "utils.h"
#include "variable.h"
#include "listIden.h"

typedef struct rangeVariable_t{
    ListIdentifier listIdentifier;
    int rangeLevel;

    struct rangeVariable_t* nextLevel;
    struct rangeVariable_t* previousLevel;
} *RangeVariable;

typedef struct {
    RangeVariable cursor;
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
RangeVariable initRangeVariable(int rangeLevel, RangeVariable previousLevel);

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
 * \fn ListRangeVariable addRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui ajoute un niveau de portée à la liste de structure de portée de variable
 *
 * \param addr : ListRangeVariable, la liste de structure de portée de variable
 *
 * \return ListRangeVariable, un pointeur de la liste de portée de variable avec un niveau de plus
*/
ListRangeVariable addRangeVariable(ListRangeVariable addr);

/*!
 * \fn ListRangeVariable deleteRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui supprime un niveau de portée à la liste de structure de portée de variable
 *
 * \param addr : ListRangeVariable, la liste de structure de portée de variable
 *
 *  \return ListRangeVariable, un pointeur de la liste de portée de variable avec un niveau de moins
*/
ListRangeVariable deleteRangeVariable(ListRangeVariable addr);

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

#endif