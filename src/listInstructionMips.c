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

/*!
 * \fn void cleanData(Data addr)
 * \brief Fonction qui libère la mémoire de la structure data
*/
void cleanData(Data addr)
{
    log_trace("cleanData (Data %p)",addr)
    CHECKPOINTER(addr);

    Data tmp, addrToFree = addr;
    while(addrToFree != NULL){
        tmp = addrToFree->previousData;
        free(addr->lineData);
        free(addrToFree);
        addrToFree = tmp;
    }

    free(addr);
}

/*!
 * \fn void cleanText(Text addr)
 * \brief Fonction qui libère la mémoire de la structure text
*/
void cleanText(Text addr)
{
    log_trace("cleanText (Text %p)",addr)
    CHECKPOINTER(addr);

    Text tmp, addrToFree = addr;
    while(addrToFree != NULL){
        tmp = addrToFree->previousText;
        free(addr->lineText);
        free(addrToFree);
        addrToFree = tmp;
    }

    free(addr);
}

/*!
 * \fn void cleanCode(Code addr)
 * \brief Fonction qui libère la mémoire de la structure code
*/
void cleanCode(Code addr)
{
    log_trace("cleanCode (Code %p)",addr)
    CHECKPOINTER(addr);

    Code tmp, addrToFree = addr;
    while(addrToFree != NULL){
        tmp = addrToFree->previousCode;
        free(addr->lineCode);
        free(addr->unDefineGoto);
        free(addrToFree);
        addrToFree = tmp;
    }

    free(addr);
}

/*!
 * \fn void cleanListInstruction(ListInstruction addr)
 * \brief Fonction qui libère la mémoire de la liste d'instruction
*/
void cleanListInstruction(ListInstruction addr)
{
    log_trace("cleanListInstruction (ListInstruction %p)",addr)
    CHECKPOINTER(addr);

    cleanData(addr->cursorData);
    cleanText(addr->cursorText);
    cleanCode(addr->cursorCode);

    free(addr);
}

/*!
 * \fn void addIntoData( ListInstruction addr, char* data)
 * \brief Fonction qui initialise une nouvelle structure de data
*/
void addStructData( ListInstruction addr)
{
    log_trace("addStructData (ListInstruction %p)",addr)
    CHECKPOINTER(addr);

    addr->cursorData = initData(addr->cursorData);
}

/*!
 * \fn void addIntoData( ListInstruction addr, char* data)
 * \brief Fonction qui permet d'ajoute en fin de la structure de data
*/
void addIntoData( ListInstruction addr, char* data)
{
    log_trace("addIntoData (ListInstruction %p, char* %s)",addr, data)
    CHECKPOINTER(addr);
    CHECKPOINTER(data);

    if(addr->cursorData->numberData >= DATA_TAB_MAX){
        log_info("struct data is full, numberData %d", addr->cursorData->numberData)
        addStructData(addr);
    }

    ulong size = strlen( data ) + 1;
    CHECKPOINTER(addr->cursorData->lineData[addr->cursorData->numberData] = (char*)malloc(sizeof(char) * size));
    CHECKPOINTER(strcpy(addr->cursorData->lineData[addr->cursorData->numberData],data));

    addr->cursorData->numberData++;
}