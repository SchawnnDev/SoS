//
// Created by the_s on 26/12/2022.
//

#ifndef SOS_LISTINSTRUCTIONMIPS_H
#define SOS_LISTINSTRUCTIONMIPS_H

#include "utils.h"
#include "variable.h"

typedef struct data_t{
    int numberData;
    char* lineData[DATA_TAB_MAX];
    struct data_t* previousData;
} *Data;

typedef struct text_t{
    int numberText;
    char* lineText[TEXT_TAB_MAX];
    struct text_t* previousText;
} *Text;

typedef struct code_t{
    int numberCode;
    char* lineCode[CODE_TAB_MAX];
    int unDefineGoto[CODE_TAB_MAX];
    struct code_t* previousCode;
} *Code;

typedef struct{
    Data cursorData;
    Text cursorText;
    Code cursorCode;
}listInstruction_t, *ListInstruction;

/*!
 * \fn Data initData( Data previousData)
 * \brief Fonction qui initialise la structure de data
 *
 * \param previousData : Data, la table précédante
 *
 * \return Data, un pointeur d'une structure de data
*/
Data initData( Data previousData);

/*!
 * \fn Text initText( Text previousText)
 * \brief Fonction qui initialise la structure de Text
 *
 * \param previousText : Text, la table précédante
 *
 * \return Text, un pointeur d'une structure de text
*/
Text initText( Text previousText);

/*!
 * \fn Code initCode( Code previousCode)
 * \brief Fonction qui initialise la structure de code
 *
 * \param previousCode : Data, la table précédante
 *
 * \return Code, un pointeur d'une structure de code
*/
Code initCode( Code previousCode);

/*!
 * \fn ListInstruction initListInstruction()
 * \brief Fonction qui initialise la structure de liste d'instruction
 *
 * \return ListInstruction, un pointeur d'une structure de liste d'instruction
*/
ListInstruction initListInstruction();

/*!
 * \fn void cleanData(Data addr)
 * \brief Fonction qui libère la mémoire de la structure data
 *
 * \param addr : ListInstruction, la structure text
*/
void cleanData(Data addr);

/*!
 * \fn void cleanText(Text addr)
 * \brief Fonction qui libère la mémoire de la structure text
 *
 * \param addr : ListInstruction, la structure d'instruction
*/
void cleanText(Text addr);

/*!
 * \fn void cleanCode(Code addr)
 * \brief Fonction qui libère la mémoire de la structure code
 *
 * \param addr : Code, la structure code
*/
void cleanCode(Code addr);

/*!
 * \fn void cleanListInstruction(ListInstruction addr)
 * \brief Fonction qui libère la mémoire de la liste d'instruction
 *
 * \param addr : ListInstruction, la structure d'instruction
*/
void cleanListInstruction(ListInstruction addr);

/*!
 * \fn void addIntoData( ListInstruction addr, char* data)
 * \brief Fonction qui initialise une nouvelle structure de data
 *
 * \param addr : Data, la table précédante
*/
void addStructData( ListInstruction addr);

/*!
 * \fn void addIntoData( ListInstruction addr, char* data)
 * \brief Fonction qui permet d'ajoute en fin de la structure de data
 *
 * \param addr : Data, la table précédante
 * \param data : char*, le code mips à stocker
*/
void addIntoData( ListInstruction addr, char* data);

#endif //SOS_LISTINSTRUCTIONMIPS_H
