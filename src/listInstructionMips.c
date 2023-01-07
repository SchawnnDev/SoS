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
    if(previousCode == NULL){
        addr->rangeCode = 0;
    } else {
        addr->rangeCode = previousCode->rangeCode +1;
    }
    return addr;
}

/*!
 * \fn GoTo initGoTo(GoTo previousGoTo)
 * \brief Fonction qui initialise la structure de code
 *
 * \param previousGoTo : GoTo, la table précédante
 *
 * \return GoTo, un pointeur d'une structure de goto
*/
GoTo initGoTo(GoTo previousGoTo)
{
    log_trace("initGoTo (GoTo %p)", previousGoTo)

    GoTo addr;
    CHECKPOINTER(addr = (GoTo) malloc(sizeof(struct goTo_t)));
    addr->previousGoTo = previousGoTo;
    addr->numberGoto = 0;
    addr->numberTrue = 0;
    addr->numberFalse = 0;
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
    addr->cursorGoTo = initListGoTo();
    return addr;
}

/*!
 * \fn ListGoTo initListGoTo()
 * \brief Fonction qui initialise la structure de liste de GoTo
 *
 * \return ListGoTo, un pointeur d'une structure de liste de GoTo
*/
ListGoTo initListGoTo()
{
    log_trace("initListGoTo (void)")

    ListGoTo addr;
    CHECKPOINTER(addr = (ListGoTo) malloc(sizeof(struct listGoTo_t)));
    addr->cursor = initGoTo(NULL);
    addr->previousListGoTo = NULL;
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
 * \fn void cleanGoTo(GoTo addr)
 * \brief Fonction qui libère la mémoire de la structure GoTo
 *
 * \param addr : GoTo, la structure code
*/
void cleanGoTo(GoTo addr)
{
    log_trace("cleanGoTo (GoTo %p)", addr)
    CHECKPOINTER(addr);

    GoTo tmp, addrToFree = addr;
    while (addrToFree != NULL)
    {
        tmp = addrToFree->previousGoTo;
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
 * \fn void cleanListGoTo(ListGoTo addr)
 * \brief Fonction qui libère la mémoire de la liste de goto
 *
 * \param addr : ListGoTo, la structure de goto
*/
void cleanListGoTo(ListGoTo addr)
{
    log_trace("cleanListGoTo (ListGoTo %p)", addr)
    CHECKPOINTER(addr);

    ListGoTo tmp, addrToFree = addr;
    while (addrToFree != NULL)
    {
        tmp = addrToFree->previousListGoTo;
        cleanGoTo(addrToFree->cursor);
        addrToFree = tmp;
    }

    free(addr);
}

/*!
 * \fn void cleanOneListGoTo(ListGoTo addr)
 * \brief Fonction qui libère la mémoire de la liste de goto
*/
void cleanOneListGoTo(ListGoTo addr)
{
    log_trace("cleanOneListGoTo (ListGoTo %p)", addr)
    CHECKPOINTER(addr);

    cleanGoTo(addr->cursor);
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
 * \fn void addStructGoTo(ListInstruction addr)
 * \brief Fonction qui initialise une nouvelle structure de goto
*/
void addStructGoTo(ListInstruction addr)
{
    log_trace("addStructGoTo (ListInstruction %p)", addr)
    CHECKPOINTER(addr);

    addr->cursorGoTo->cursor = initGoTo(addr->cursorGoTo->cursor);
}

/*!
 * \fn void addStructListGoTo(ListInstruction addr)
 * \brief Fonction qui initialise une nouvelle structure de goto
*/
int addStructListGoTo(ListInstruction addr)
{
    log_trace("addStructListGoTo(ListInstruction %p)", addr)
    CHECKPOINTER(addr);

    ListGoTo tmp = initListGoTo();
    tmp->previousListGoTo = addr->cursorGoTo;
    addr->cursorGoTo = tmp;

    return RETURN_SUCCESS;
}

/*!
 * \fn void addStructListGoTo(ListInstruction addr)
 * \brief Fonction qui initialise une nouvelle structure de goto
*/
int deleteStructListGoTo(ListInstruction addr)
{
    log_trace("deleteStructListGoTo(ListInstruction %p)", addr)
    CHECKPOINTER(addr);

    ListGoTo tmp = addr->cursorGoTo;
    addr->cursorGoTo = tmp->previousListGoTo;
    cleanOneListGoTo(tmp);

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

    if (addr->cursorCode->numberCode >= CODE_TAB_MAX)
    {
        log_info("struct Code is full, numberCode %d",
                 addr->cursorCode->numberCode)
        addStructCode(addr);
    }
    if (addr->cursorGoTo->cursor->numberTrue >= CODE_TAB_MAX)
    {
        log_info("struct GoTo is full, numberTrue %d",
                 addr->cursorGoTo->cursor->numberTrue)
        addStructGoTo(addr);
    }

    int index = addr->cursorCode->numberCode + addr->cursorCode->rangeCode * CODE_TAB_MAX;
    addr->cursorGoTo->cursor->trueList[addr->cursorGoTo->cursor->numberTrue] = index;
    addr->cursorGoTo->cursor->numberTrue++;
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

    if (addr->cursorCode->numberCode >= CODE_TAB_MAX)
    {
        log_info("struct Code is full, numberCode %d",
                 addr->cursorCode->numberCode)
        addStructCode(addr);
    }
    if (addr->cursorGoTo->cursor->numberFalse >= CODE_TAB_MAX)
    {
        log_info("struct GoTo is full, numberTrue %d",
                 addr->cursorGoTo->cursor->numberFalse)
        addStructGoTo(addr);
    }

    int index = addr->cursorCode->numberCode + addr->cursorCode->rangeCode * CODE_TAB_MAX;
    addr->cursorGoTo->cursor->falseList[addr->cursorGoTo->cursor->numberFalse] = index;
    addr->cursorGoTo->cursor->numberFalse++;
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

    if (addr->cursorCode->numberCode >= CODE_TAB_MAX)
    {
        log_info("struct Code is full, numberCode %d",
                 addr->cursorCode->numberCode)
        addStructCode(addr);
    }
    if (addr->cursorGoTo->cursor->numberGoto >= CODE_TAB_MAX)
    {
        log_info("struct GoTo is full, numberTrue %d",
                 addr->cursorGoTo->cursor->numberGoto)
        addStructGoTo(addr);
    }

    int index = addr->cursorCode->numberCode + addr->cursorCode->rangeCode * CODE_TAB_MAX;
    addr->cursorGoTo->cursor->unDefineGoto[addr->cursorGoTo->cursor->numberGoto] = index;
    addr->cursorGoTo->cursor->numberGoto++;
    addIntoCode(addr, code);
}

/*!
 * \fn void completeTrueList( ListInstruction addr, char* code )
 * \brief Fonction qui permet complete une ligne de la trueList
*/
int completeTrueList(ListInstruction addr, char *code)
{
    log_trace("completeTrueList (ListInstruction %p, char* %s)", addr, code)
    CHECKPOINTER(addr);
    CHECKPOINTER(code);

    GoTo tmp = addr->cursorGoTo->cursor;
    while ((tmp != NULL) && (tmp->numberTrue == 0)){
        tmp = tmp->previousGoTo;
    }

    if(tmp == NULL){
        log_trace("completeTrueList : there is no goto to complete")
        return RETURN_FAILURE;
    }

    int range = tmp->trueList[tmp->numberTrue-1];
    int index = range % CODE_TAB_MAX;
    range = (range - index) / CODE_TAB_MAX;
    Code tmpCode = addr->cursorCode;
    while ((tmpCode != NULL) && (tmpCode->rangeCode != range)){
        tmpCode = tmpCode->previousCode;
    }

    if(tmpCode == NULL){
        log_trace("completeTrueList : there is no goto to complete")
        return RETURN_FAILURE;
    }

    tmp->numberTrue--;
    char * newCode;
    int size2 = strlen(code)+1;
    int size1 = strlen(tmpCode->lineCode[index]);
    if(size2 == 1){
        log_trace("completeTrueList : code is empty")
        return RETURN_FAILURE;
    }
    size1 = size1 + size2 + 2;

    CHECKPOINTER(newCode = (char*) malloc(sizeof (char) * size1))
    CHECK(sprintf(newCode,"%s", tmpCode->lineCode[index]))
    CHECKPOINTER(strcat(newCode," "))
    CHECKPOINTER(strcat(newCode,code))

    free(tmpCode->lineCode[index]);
    tmpCode->lineCode[index] = newCode;

    return RETURN_SUCCESS;
}

/*!
 * \fn int completeFalseList( ListInstruction addr, char* code )
 * \brief Fonction qui permet complete une ligne de la falseList
*/
int completeFalseList(ListInstruction addr, char *code)
{
    log_trace("completeFalseList (ListInstruction %p, char* %s)", addr, code)
    CHECKPOINTER(addr);
    CHECKPOINTER(code);

    GoTo tmp = addr->cursorGoTo->cursor;
    while ((tmp != NULL) && (tmp->numberFalse == 0)){
        tmp = tmp->previousGoTo;
    }

    if(tmp == NULL){
        log_trace("completeFalseList : there is no goto to complete")
        return RETURN_FAILURE;
    }

    int range = tmp->falseList[tmp->numberFalse-1];
    int index = range % CODE_TAB_MAX;
    range = (range - index) / CODE_TAB_MAX;
    Code tmpCode = addr->cursorCode;
    while ((tmpCode != NULL) && (tmpCode->rangeCode != range)){
        tmpCode = tmpCode->previousCode;
    }

    if(tmpCode == NULL){
        log_trace("completeTrueList : there is no goto to complete")
        return RETURN_FAILURE;
    }

    tmp->numberFalse--;

    char * newCode;
    int size2 = strlen(code)+1;
    int size1 = strlen(tmpCode->lineCode[index]);

    if(size2 == 1){
        log_trace("completeFalseList : code is empty")
        newCode = "";
    } else {
        size1 = size1 + size2 + 2;

        CHECKPOINTER(newCode = (char*) malloc(sizeof (char) * size1))
        CHECK(sprintf(newCode,"%s", tmpCode->lineCode[index]))
        CHECKPOINTER(strcat(newCode," "))
        CHECKPOINTER(strcat(newCode,code))
    }

    free(tmpCode->lineCode[index]);
    tmpCode->lineCode[index] = newCode;

    return RETURN_SUCCESS;
}

/*!
 * \fn int completeUnDefineGoto( ListInstruction addr, char* code )
 * \brief Fonction qui permet d'ajoute un goto indéterminé
*/
int completeUnDefineGoto(ListInstruction addr, char *code)
{
    log_trace("completeUnDefineGoto (ListInstruction %p, char* %s)", addr, code)
    CHECKPOINTER(addr);
    CHECKPOINTER(code);

    GoTo tmp = addr->cursorGoTo->cursor;
    while(tmp != NULL){
        while ((tmp != NULL) && (tmp->numberGoto == 0)){
            tmp = tmp->previousGoTo;
        }

        if(tmp == NULL){
            log_trace("completeUnDefineGoto : there is no goto to complete")
            return RETURN_FAILURE;
        }

        int range = tmp->unDefineGoto[tmp->numberGoto-1];
        int index = range % CODE_TAB_MAX;
        range = (range - index) / CODE_TAB_MAX;
        Code tmpCode = addr->cursorCode;
        while ((tmpCode != NULL) && (tmpCode->rangeCode != range)){
            tmpCode = tmpCode->previousCode;
        }

        if(tmpCode == NULL){
            log_trace("completeTrueList : there is no goto to complete")
            return RETURN_FAILURE;
        }

        char * newCode;
        int size2 = strlen(code)+1;
        int size1 = strlen(tmpCode->lineCode[index]);
        if(size1 >= 2){
            if(size2 == 1){
                log_trace("completeUnDefineGoto : code is empty")
                return RETURN_FAILURE;
            }
            tmp->numberGoto--;
            size1 = size1 + size2 + 2;

            CHECKPOINTER(newCode = (char*) malloc(sizeof (char) * size1))
            CHECK(sprintf(newCode,"%s", tmpCode->lineCode[index]))
            CHECKPOINTER(strcat(newCode," "))
            CHECKPOINTER(strcat(newCode,code))
        } else {
            tmp->numberGoto--;
            if(size2 == 1){
                log_trace("completeUnDefineGoto : code is empty")
                return RETURN_FAILURE;
            }
            size1 = size2 + 4;

            CHECKPOINTER(newCode = (char*) malloc(sizeof (char) * size1))
            CHECK(sprintf(newCode,"%s", tmpCode->lineCode[index]))
            CHECKPOINTER(strcat(newCode,code))
            CHECKPOINTER(strcat(newCode,":"))
        }

        free(tmpCode->lineCode[index]);
        tmpCode->lineCode[index] = newCode;
    }

    return RETURN_SUCCESS;
}

/*!
 * \fn void completeOneUnDefineGoto( ListInstruction addr, char* code )
 * \brief Fonction qui permet d'ajoute un goto indéterminé
*/
int completeOneUnDefineGoto(ListInstruction addr, char *code)
{
    log_trace("completeUnDefineGoto (ListInstruction %p, char* %s)", addr, code)
    CHECKPOINTER(addr);
    CHECKPOINTER(code);

    GoTo tmp = addr->cursorGoTo->cursor;
    while ((tmp != NULL) && (tmp->numberGoto == 0)){
        tmp = tmp->previousGoTo;
    }

    if(tmp == NULL){
        log_trace("completeUnDefineGoto : there is no goto to complete")
        return RETURN_FAILURE;
    }

    int range = tmp->unDefineGoto[tmp->numberGoto-1];
    int index = range % CODE_TAB_MAX;
    range = (range - index) / CODE_TAB_MAX;
    Code tmpCode = addr->cursorCode;
    while ((tmpCode != NULL) && (tmpCode->rangeCode != range)){
        tmpCode = tmpCode->previousCode;
    }

    if(tmpCode == NULL){
        log_trace("completeTrueList : there is no goto to complete")
        return RETURN_FAILURE;
    }

    int returnValue = RETURN_SUCCESS;
    char * newCode;
    int size2 = strlen(code)+1;
    int size1 = strlen(tmpCode->lineCode[index]);
    if(size1 >= 2){
        if(size2 == 1){
            log_trace("completeUnDefineGoto : code is empty")
            return RETURN_FAILURE;
        }
        tmp->numberGoto--;
        size1 = size1 + size2 + 2;

        CHECKPOINTER(newCode = (char*) malloc(sizeof (char) * size1))
        CHECK(sprintf(newCode,"%s", tmpCode->lineCode[index]))
        CHECKPOINTER(strcat(newCode," "))
        CHECKPOINTER(strcat(newCode,code))
    } else {
        tmp->numberGoto--;
        if(size2 == 1){
            log_trace("completeUnDefineGoto : code is empty")
            return RETURN_FAILURE;
        }
        size1 = size2 + 4;

        CHECKPOINTER(newCode = (char*) malloc(sizeof (char) * size1))
        CHECK(sprintf(newCode,"%s", tmpCode->lineCode[index]))
        CHECKPOINTER(strcat(newCode,code))
        CHECKPOINTER(strcat(newCode,":"))
        returnValue++;
    }

    free(tmpCode->lineCode[index]);
    tmpCode->lineCode[index] = newCode;

    return returnValue;
}

/*!
 * \fn int switchTrueFalseList(ListInstruction addr)
 * \brief Fonction qui switch la trueList et la falseList
*/
int switchTrueFalseList(ListInstruction addr)
{
    int tmpTab;

    GoTo tmpGoTo = addr->cursorGoTo->cursor;
    while ((tmpGoTo != NULL) && (tmpGoTo->numberGoto == 0)){
        tmpGoTo = tmpGoTo->previousGoTo;
    }

    if(tmpGoTo == NULL){
        log_trace("switchTrueFalseList : there is no goto to complete")
        return RETURN_FAILURE;
    }

    tmpGoTo->numberTrue--;
    tmpTab = tmpGoTo->trueList[tmpGoTo->numberTrue];

    tmpGoTo->numberFalse--;
    tmpGoTo->trueList[tmpGoTo->numberTrue] = tmpGoTo->falseList[tmpGoTo->numberFalse];
    tmpGoTo->numberTrue++;

    tmpGoTo->falseList[tmpGoTo->numberFalse] = tmpTab;
    tmpGoTo->numberFalse++;
    
    return RETURN_SUCCESS;
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