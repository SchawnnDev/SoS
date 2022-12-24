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

typedef struct identifierOrder_t {
    char* name;
    int type;
    int index;
    struct identifierOrder_t* previousIdentifier;
} *IdentifierOrder;

typedef struct {
    IdentifierOrder cursor;
} listIdentifierOrder_t, *ListIdentifierOrder;

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
 * \fn int addRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui ajoute un niveau de portée à la liste de structure de portée de variable
 *
 * \param addr : ListRangeVariable, la liste de structure de portée de variable
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int addRangeVariable(ListRangeVariable addr);

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
int addIdentifier(ListRangeVariable addr, char* name);

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
 * \fn int setValuesFromListTmp(ListRangeVariable addr, char* name, ListTmp addrTmp)
 * \brief Fonction remplie le tableau des valeurs de l'identificateur dans la liste des postée de variable depuis à la liste temporaire
 *
 * \param addr : ListRangeVariable, la liste de structure de portée de variable
 * \param name : char*, le nom de l'identificateur
 * \param ListTmp : addrTmp, la liste des valeurs temporaire
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int setValuesFromListTmp(ListRangeVariable addr, char* name, ListTmp addrTmp);

/*!
 * \fn IdentifierOrder initIdentifierOrder(IdentifierOrder previousLevel, char* name)
 * \brief Fonction qui initialise la structure d'ordre d'apparition des identificateurs
 *
 * \param previousLevel : previousIdentifier, l'identificateur précédant
 * \param name : char*, le nom de l'identificateur
 *
 * \return IdentifierOrder, un pointeur d'une structure d'ordre d'apparition des identificateurs
*/
IdentifierOrder initIdentifierOrder(IdentifierOrder previousIdentifier, char* name);

/*!
 * \fn void cleanIdentifierOrder(IdentifierOrder addr)
 * \brief Fonction qui libère la mémoire d'une structure d'ordre d'apparition des identificateurs
 *
 * \param addr : IdentifierOrder, la structure d'ordre d'apparition des identificateurs
*/
void cleanIdentifierOrder(IdentifierOrder addr);

/*!
 * \fn ListRangeVariable initListRangeVariable()
 * \brief Fonction qui initialise la liste de structure d'ordre d'apparition des identificateurs
 *
 * \return ListRangeVariable, un pointeur d'une liste de structure d'ordre d'apparition des identificateurs
*/
ListIdentifierOrder initListIdentifierOrder();

/*!
 * \fn void cleanListRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui libère la mémoire d'une liste de structure d'ordre d'apparition des identificateurs
 *
 * \param addr : ListRangeVariable, la liste de structure d'ordre d'apparition des identificateurs
*/
void cleanListIdentifierOrder(ListIdentifierOrder addr);

/*!
 * \fn void addIdentifierOrder(ListIdentifierOrder addr, char * name)
 * \brief Fonction qui ajoute un niveau de portée à la liste d'ordre d'apparition des identificateurs
 *
 * \param addr : ListRangeVariable, la liste de structure d'ordre d'apparition des identificateurse
 * \param name : char*, le nom de l'identificateur
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
void addIdentifierOrder(ListIdentifierOrder addr, char * name);

/*!
 * \fn int setTypeIdentifierOrder(ListIdentifierOrder addr, int type)
 * \brief Fonction qui modifie le type du dernier identificateur de la liste de structure d'ordre d'apparition des identificateurs
 *
 * \param addr : ListIdentifierOrder, la liste de structure d'ordre d'apparition des identificateurs
 * \param type : int, le type de l'identificateur
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int setTypeIdentifierOrder(ListIdentifierOrder addr, int type);

/*!
 * \fn int setIndexIdentifierOrder(ListIdentifierOrder addr, int index)
 * \brief Fonction qui modifie l'index pour l'affectation d'un tableau du dernier identificateur de la liste de structure d'ordre d'apparition des identificateurs
 *
 * \param addr : ListIdentifierOrder, la liste de structure d'ordre d'apparition des identificateurs
 * \param index : int, l'index de l'identificateur
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int setIndexIdentifierOrder(ListIdentifierOrder addr, int index);

/*!
 * \fn int deleteIdentifierOrder(ListIdentifierOrder addr)
 * \brief Fonction qui supprime un niveau de portée à la liste de structure d'ordre d'apparition des identificateurs
 *
 * \param addr : ListRangeVariable, la liste de structure d'ordre d'apparition des identificateurs
 *
 *  \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int deleteIdentifierOrder(ListIdentifierOrder addr);


#endif