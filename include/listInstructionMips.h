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
 * \return Data, un pointeur d'une structure de data
*/
ListInstruction initListInstruction();

#endif //SOS_LISTINSTRUCTIONMIPS_H
