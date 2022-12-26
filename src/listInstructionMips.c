#include "listInstructionMips.h"

/*!
 * \fn Data initData( Data previousData)
 * \brief Fonction qui initialise la structure de data
*/
Data initData(Data previousData)
{
    log_trace("initData (Data %p)",previousData)

    Data addr;
    CHECKPOINTER(addr = (Data)malloc(sizeof( struct data_t)));
    addr->previousData = previousData;
    addr->numberData = 0;
    return addr;
}

/*!
 * \fn Text initText( Text previousText)
 * \brief Fonction qui initialise la structure de Text
*/
Text initText(Text previousText)
{
    log_trace("initText (Text %p)",previousText)

    Text addr;
    CHECKPOINTER(addr = (Text)malloc(sizeof( struct text_t)));
    addr->previousText = previousText;
    addr->numberText = 0;
    return addr;
}

/*!
 * \fn Code initCode( Code previousCode)
 * \brief Fonction qui initialise la structure de code
*/
Code initCode(Code previousCode)
{
    log_trace("initCode (Code %p)",previousCode)

    Code addr;
    CHECKPOINTER(addr = (Code)malloc(sizeof( struct code_t)));
    addr->previousCode = previousCode;
    addr->numberCode = 0;
    return addr;
}

/*!
 * \fn ListInstruction initListInstruction()
 * \brief Fonction qui initialise la structure de liste d'instruction
*/
ListInstruction initListInstruction()
{
    log_trace("initListInstruction (void)")

    ListInstruction addr;
    CHECKPOINTER(addr = (ListInstruction)malloc(sizeof( listInstruction_t)));
    addr->cursorData = initData(NULL);
    addr->cursorText = initText(NULL);
    addr->cursorCode = initCode(NULL);
    return addr;
}