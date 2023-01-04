#include "listInstructionMips.h"

/*!
 * \fn Data initData( Data previousData)
 * \brief Fonction qui initialise la structure de data
*/
Data initData(Data previousData)
{
    log_trace("initData (Data %p)", previousData)

    Data addr;
    CHECKPOINTER(addr = (Data) malloc(sizeof(struct data_t)));
    addr->previousData = previousData;
    addr->nextData = NULL;
    addr->numberData = 0;
    return addr;
}

/*!
 * \fn Code initCode( Code previousCode, int codeLevel)
 * \brief Fonction qui initialise la structure de code
*/
Code initCode(Code previousCode)
{
    log_trace("initCode (Code %p)", previousCode)

    Code addr;
    CHECKPOINTER(addr = (Code) malloc(sizeof(struct code_t)));
    addr->previousCode = previousCode;
    addr->nextCode = NULL;
    addr->numberCode = 0;
    addr->numberGoto = 0;
    addr->numberTrue = 0;
    addr->numberFalse = 0;
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
    CHECKPOINTER(addr = (ListInstruction) malloc(sizeof(listInstruction_t)));
    addr->cursorData = initData(NULL);
    addr->cursorCode = initCode(NULL);
    return addr;
}

/*!
 * \fn void cleanData(Data addr)
 * \brief Fonction qui libère la mémoire de la structure data
*/
void cleanData(Data addr)
{
    log_trace("cleanData (Data %p)", addr)
    CHECKPOINTER(addr);

    Data tmp, addrToFree = addr;
    while (addrToFree != NULL)
    {
        tmp = addrToFree->previousData;
        free(addrToFree);
        addrToFree = tmp;
    }
}

/*!
 * \fn void cleanCode(Code addr)
 * \brief Fonction qui libère la mémoire de la structure code
*/
void cleanCode(Code addr)
{
    log_trace("cleanCode (Code %p)", addr)
    CHECKPOINTER(addr);

    Code tmp, addrToFree = addr;
    while (addrToFree != NULL)
    {
        tmp = addrToFree->previousCode;
        free(addrToFree);
        addrToFree = tmp;
    }
}

/*!
 * \fn void cleanListInstruction(ListInstruction addr)
 * \brief Fonction qui libère la mémoire de la liste d'instruction
*/
void cleanListInstruction(ListInstruction addr)
{
    log_trace("cleanListInstruction (ListInstruction %p)", addr)
    CHECKPOINTER(addr);

    cleanData(addr->cursorData);
    cleanCode(addr->cursorCode);

    free(addr);
}

/*!
 * \fn void addIntoData( ListInstruction addr, char* data)
 * \brief Fonction qui initialise une nouvelle structure de data
*/
void addStructData(ListInstruction addr)
{
    log_trace("addStructData (ListInstruction %p)", addr)
    CHECKPOINTER(addr);

    addr->cursorData = initData(addr->cursorData);
    addr->cursorData->previousData->nextData = addr->cursorData;
}

/*!
 * \fn void addIntoData( ListInstruction addr, char* data)
 * \brief Fonction qui permet d'ajoute en fin de la structure de data
*/
void addIntoData(ListInstruction addr, char *data)
{
   // log_trace("addIntoData (ListInstruction %p, char* %s)", addr, data)
    CHECKPOINTER(addr);
    CHECKPOINTER(data);

    if (addr->cursorData->numberData >= DATA_TAB_MAX)
    {
        log_info("struct data is full, numberData %d",
                 addr->cursorData->numberData)
        addStructData(addr);
    }

    ulong size = strlen(data) + 1;
    CHECKPOINTER(
            addr->cursorData->lineData[addr->cursorData->numberData] = (char *) malloc(
                    sizeof(char) * size));
    CHECKPOINTER(
            strcpy(addr->cursorData->lineData[addr->cursorData->numberData],
                   data));

    addr->cursorData->numberData++;
}

/*!
 * \fn void addStructCode( ListInstruction addr)
 * \brief Fonction qui initialise une nouvelle structure de code
*/
void addStructCode(ListInstruction addr)
{
    log_trace("addStructCode (ListInstruction %p)", addr)
    CHECKPOINTER(addr);

    addr->cursorCode = initCode(addr->cursorCode);
    addr->cursorCode->previousCode->nextCode = addr->cursorCode;
}

/*!
 * \fn void addIntoCode( ListInstruction addr, char* code)
 * \brief Fonction qui permet d'ajoute en fin de la structure de code
*/
void addIntoCode(ListInstruction addr, char *code)
{
    if (addr->cursorCode->numberCode >= CODE_TAB_MAX)
    {
        log_info("struct Code is full, numberCode %d",
                 addr->cursorCode->numberCode)
        addStructCode(addr);
    }

    addIntoCodeWithIndex(addr->cursorCode, code, addr->cursorCode->numberCode);
    addr->cursorCode->numberCode++;
}

/*!
 * \fn int addIntoCode(Code addr, char* code)
 * \brief Fonction qui permet d'ajoute en fin de la structure de code
*/
int addIntoCodeWithIndex(Code addr, char *code, int index)
{
//    log_trace("addIntoCode (ListInstruction %p, char* %s)", addr, code)
    CHECKPOINTER(addr);
    CHECKPOINTER(code);

    if ((index < 0) || (index >= CODE_TAB_MAX))
    {
        log_error("index out of range : position : %d", index)
        perror("addIntoCodeWithIndex : can not set code of non-existent table index.");
        return RETURN_FAILURE;
    }

    ulong size = strlen(code) + 1;
    CHECKPOINTER(addr->lineCode[index] = (char *) malloc(sizeof(char) * size));
    CHECKPOINTER(strcpy(addr->lineCode[index], code));

    return RETURN_SUCCESS;
}

/*!
 * \fn void addIntoTrueList( ListInstruction addr, char* code)
 * \brief Fonction qui permet d'ajoute à la trueList
*/
void addIntoTrueList(ListInstruction addr, char* code)
{
    log_trace("addIntoTrueList (ListInstruction %p, char* %s)", addr, code)
    CHECKPOINTER(addr);

    addr->cursorCode->trueList[addr->cursorCode->numberTrue] = addr->cursorCode->numberCode;
    addr->cursorCode->numberTrue++;
    addIntoCode(addr, code);
}

/*!
 * \fn void addIntoFalseList( ListInstruction addr, char* code)
 * \brief Fonction qui permet d'ajoute à la falseList
*/
void addIntoFalseList(ListInstruction addr, char* code)
{
    log_trace("addIntoFalseList (ListInstruction %p, char* %s)", addr, code)
    CHECKPOINTER(addr);

    addr->cursorCode->falseList[addr->cursorCode->numberFalse] = addr->cursorCode->numberCode;
    addr->cursorCode->numberFalse++;
    addIntoCode(addr, code);
}

/*!
 * \fn void addIntoCode( ListInstruction addr, char* code)
 * \brief Fonction qui permet d'ajoute un goto indéterminé
*/
void addIntoUnDefineGoto(ListInstruction addr, char* code)
{
    log_trace("addIntoUnDefineGoto (ListInstruction %p, char* %s)", addr, code)
    CHECKPOINTER(addr);

    addr->cursorCode->unDefineGoto[addr->cursorCode->numberGoto] = addr->cursorCode->numberCode;
    addr->cursorCode->numberGoto++;
    addIntoCode(addr, code);
}
/*!
 * \fn void completeTrueList( ListInstruction addr, char* code )
 * \brief Fonction qui permet complete une ligne de la trueList
*/
void completeTrueList(ListInstruction addr, char *code)
{
    log_trace("completeTrueList (ListInstruction %p, char* %s)", addr, code)
    CHECKPOINTER(addr);
    CHECKPOINTER(code);

    if(addr->cursorCode->numberTrue == 0){
        log_trace("completeTrueList : there is no goto to complete")
        return;
    }

    int size2 = strlen(code)+1;
    addr->cursorCode->numberTrue--;
    if(size2 == 1){
        log_trace("completeTrueList : code is empty")
        return;
    }

    int size1 = strlen(addr->cursorCode->lineCode[addr->cursorCode->trueList[addr->cursorCode->numberTrue]]);
    size1 = size1 + size2 + 2;

    char * newCode;
    CHECKPOINTER(newCode = (char*) malloc(sizeof (char) * size1))
    CHECK(sprintf(newCode,addr->cursorCode->lineCode[addr->cursorCode->trueList[addr->cursorCode->numberTrue]]))
    CHECKPOINTER(strcat(newCode," "))
    CHECKPOINTER(strcat(newCode,code))

    free(addr->cursorCode->lineCode[addr->cursorCode->trueList[addr->cursorCode->numberTrue]]);
    addr->cursorCode->lineCode[addr->cursorCode->trueList[addr->cursorCode->numberTrue]] = newCode;

}


/*!
 * \fn void completeFalseList( ListInstruction addr, char* code )
 * \brief Fonction qui permet complete une ligne de la falseList
*/
void completeFalseList(ListInstruction addr, char *code)
{
    log_trace("completeFalseList (ListInstruction %p, char* %s)", addr, code)
    CHECKPOINTER(addr);
    CHECKPOINTER(code);

    if(addr->cursorCode->numberFalse == 0){
        log_trace("completeFalseList : there is no goto to complete")
        return;
    }

    int size2 = strlen(code)+1;
    addr->cursorCode->numberFalse--;
    if(size2 == 1){
        log_trace("completeFalseList : code is empty")
        return;
    }

    int size1 = strlen(addr->cursorCode->lineCode[addr->cursorCode->falseList[addr->cursorCode->numberFalse]]);
    size1 = size1 + size2 + 2;

    char * newCode;
    CHECKPOINTER(newCode = (char*) malloc(sizeof (char) * size1))
    CHECK(sprintf(newCode,addr->cursorCode->lineCode[addr->cursorCode->falseList[addr->cursorCode->numberFalse]]))
    CHECKPOINTER(strcat(newCode," "))
    CHECKPOINTER(strcat(newCode,code))

    free(addr->cursorCode->lineCode[addr->cursorCode->falseList[addr->cursorCode->numberFalse]]);
    addr->cursorCode->lineCode[addr->cursorCode->falseList[addr->cursorCode->numberFalse]] = newCode;

}

/*!
 * \fn void completeUnDefineGoto( ListInstruction addr, char* code )
 * \brief Fonction qui permet d'ajoute un goto indéterminé
*/
void completeUnDefineGoto(ListInstruction addr, char *code)
{
    log_trace("completeUnDefineGoto (ListInstruction %p, char* %s)", addr, code)
    CHECKPOINTER(addr);
    CHECKPOINTER(code);

    if(addr->cursorCode->numberGoto == 0){
        log_trace("completeUnDefineGoto : there is no goto to complete")
        return;
    }

    int size2 = strlen(code)+1;
    addr->cursorCode->numberGoto--;
    if(size2 == 1){
        log_trace("completeUnDefineGoto : code is empty")
        return;
    }

    int size1 = strlen(addr->cursorCode->lineCode[addr->cursorCode->unDefineGoto[addr->cursorCode->numberGoto]]);
    size1 = size1 + size2 + 2;

    char * newCode;
    CHECKPOINTER(newCode = (char*) malloc(sizeof (char) * size1))
    CHECK(sprintf(newCode,addr->cursorCode->lineCode[addr->cursorCode->unDefineGoto[addr->cursorCode->numberGoto]]))
    CHECKPOINTER(strcat(newCode," "))
    CHECKPOINTER(strcat(newCode,code))

    free(addr->cursorCode->lineCode[addr->cursorCode->unDefineGoto[addr->cursorCode->numberGoto]]);
    addr->cursorCode->lineCode[addr->cursorCode->unDefineGoto[addr->cursorCode->numberGoto]] = newCode;

}

Data getFirstDataCursor(Data cursor)
{
    Data found = cursor;
    while (found != NULL && found->previousData != NULL)
        found = found->previousData;
    return found;
}

Code getFirstCodeCursor(Code cursor)
{
    Code found = cursor;
    while (found != NULL && found->previousCode != NULL)
        found = found->previousCode;
    return found;
}

int writeToFile(ListInstruction list, FILE *file)
{
    Data data = getFirstDataCursor(list->cursorData);
    Code code = getFirstCodeCursor(list->cursorCode);
    CHECK(fprintf(file, ".data\n"));

    do
    {
        for (int i = 0; i < data->numberData; ++i)
        {
            CHECK(fprintf(file, "%s", data->lineData[i]));
        }
        data = data->nextData;
    } while (data != NULL);

    CHECK(fprintf(file, ".text\n"));

    do
    {
        for (int i = 0; i < code->numberCode; ++i)
        {
            CHECK(fprintf(file, "%s", code->lineCode[i]));
        }
        code = code->nextCode;
    } while (code != NULL);

    return RETURN_SUCCESS;
}