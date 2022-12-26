#include "listInstructionMips.h"

/*!
 * \fn Data initData( Data previousData)
 * \brief Fonction qui initialise la structure de data
*/
Data initData(Data previousData) {
    log_trace("initData (Data %p)", previousData)

    Data addr;
    CHECKPOINTER(addr = (Data) malloc(sizeof(struct data_t)));
    addr->previousData = previousData;
    addr->numberData = 0;
    return addr;
}

/*!
 * \fn Text initText( Text previousText)
 * \brief Fonction qui initialise la structure de Text
*/
Text initText(Text previousText) {
    log_trace("initText (Text %p)", previousText)

    Text addr;
    CHECKPOINTER(addr = (Text) malloc(sizeof(struct text_t)));
    addr->previousText = previousText;
    addr->numberText = 0;
    return addr;
}

/*!
 * \fn Code initCode( Code previousCode, int codeLevel)
 * \brief Fonction qui initialise la structure de code
*/
Code initCode(Code previousCode) {
    log_trace("initCode (Code %p)", previousCode)

    Code addr;
    CHECKPOINTER(addr = (Code) malloc(sizeof(struct code_t)));
    addr->previousCode = previousCode;
    addr->numberCode = 0;
    addr->numberGoto = 0;
    return addr;
}

/*!
 * \fn ListInstruction initListInstruction()
 * \brief Fonction qui initialise la structure de liste d'instruction
*/
ListInstruction initListInstruction() {
    log_trace("initListInstruction (void)")

    ListInstruction addr;
    CHECKPOINTER(addr = (ListInstruction) malloc(sizeof(listInstruction_t)));
    addr->cursorData = initData(NULL);
    addr->cursorText = initText(NULL);
    addr->cursorCode = initCode(NULL);
    return addr;
}

/*!
 * \fn void cleanData(Data addr)
 * \brief Fonction qui libère la mémoire de la structure data
*/
void cleanData(Data addr) {
    log_trace("cleanData (Data %p)", addr)
    CHECKPOINTER(addr);

    Data tmp, addrToFree = addr;
    while (addrToFree != NULL) {
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
void cleanText(Text addr) {
    log_trace("cleanText (Text %p)", addr)
    CHECKPOINTER(addr);

    Text tmp, addrToFree = addr;
    while (addrToFree != NULL) {
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
void cleanCode(Code addr) {
    log_trace("cleanCode (Code %p)", addr)
    CHECKPOINTER(addr);

    Code tmp, addrToFree = addr;
    while (addrToFree != NULL) {
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
void cleanListInstruction(ListInstruction addr) {
    log_trace("cleanListInstruction (ListInstruction %p)", addr)
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
void addStructData(ListInstruction addr) {
    log_trace("addStructData (ListInstruction %p)", addr)
    CHECKPOINTER(addr);

    addr->cursorData = initData(addr->cursorData);
}

/*!
 * \fn void addIntoData( ListInstruction addr, char* data)
 * \brief Fonction qui permet d'ajoute en fin de la structure de data
*/
void addIntoData(ListInstruction addr, char *data) {
    log_trace("addIntoData (ListInstruction %p, char* %s)", addr, data)
    CHECKPOINTER(addr);
    CHECKPOINTER(data);

    if (addr->cursorData->numberData >= DATA_TAB_MAX) {
        log_info("struct data is full, numberData %d", addr->cursorData->numberData)
        addStructData(addr);
    }

    ulong size = strlen(data) + 1;
    CHECKPOINTER(addr->cursorData->lineData[addr->cursorData->numberData] = (char *) malloc(sizeof(char) * size));
    CHECKPOINTER(strcpy(addr->cursorData->lineData[addr->cursorData->numberData], data));

    addr->cursorData->numberData++;
}

/*!
 * \fn void addIntoText( ListInstruction addr)
 * \brief Fonction qui initialise une nouvelle structure de text
 *
 * \param addr : Data, la table précédante
*/
void addStructText(ListInstruction addr) {
    log_trace("addStructText (ListInstruction %p)", addr)
    CHECKPOINTER(addr);

    addr->cursorText = initText(addr->cursorText);
}

/*!
 * \fn void addIntoText( ListInstruction addr, char* text)
 * \brief Fonction qui permet d'ajoute en fin de la structure de text
 *
 * \param addr : Data, la table précédante
 * \param text : char*, le code mips à stocker
*/
void addIntoText(ListInstruction addr, char *text) {
    log_trace("addIntoText (ListInstruction %p, char* %s)", addr, text)
    CHECKPOINTER(addr);
    CHECKPOINTER(text);

    if (addr->cursorText->numberText >= TEXT_TAB_MAX) {
        log_info("struct Text is full, numberText %d", addr->cursorText->numberText)
        addStructText(addr);
    }

    ulong size = strlen(text) + 1;
    CHECKPOINTER(addr->cursorText->lineText[addr->cursorText->numberText] = (char *) malloc(sizeof(char) * size));
    CHECKPOINTER(strcpy(addr->cursorText->lineText[addr->cursorText->numberText], text));

    addr->cursorText->numberText++;
}

/*!
 * \fn void addStructCode( ListInstruction addr)
 * \brief Fonction qui initialise une nouvelle structure de code
 *
 * \param addr : ListInstruction, la structure d'instruction
*/
void addStructCode(ListInstruction addr) {
    log_trace("addStructCode (ListInstruction %p)", addr)
    CHECKPOINTER(addr);

    addr->cursorCode = initCode(addr->cursorCode);
}

/*!
 * \fn void addIntoCode( ListInstruction addr, char* code)
 * \brief Fonction qui permet d'ajoute en fin de la structure de code
 *
 * \param addr : ListInstruction, la structure d'instruction
 * \param code : char*, le code mips à stocker
*/
void addIntoCode(ListInstruction addr, char *code) {
    log_trace("addIntoCode (ListInstruction %p, char* %s)", addr, code)
    CHECKPOINTER(addr);
    CHECKPOINTER(code);

    if (addr->cursorCode->numberCode >= TEXT_TAB_MAX) {
        log_info("struct Code is full, numberCode %d", addr->cursorCode->numberCode)
        addStructCode(addr);
    }

    ulong size = strlen(code) + 1;
    CHECKPOINTER(addr->cursorCode->lineCode[addr->cursorCode->numberCode] = (char *) malloc(sizeof(char) * size));
    CHECKPOINTER(strcpy(addr->cursorCode->lineCode[addr->cursorCode->numberCode], code));

    addr->cursorCode->numberCode++;
}

/*!
 * \fn void addIntoCode( ListInstruction addr, char* code)
 * \brief Fonction qui permet d'ajoute un goto indéterminé
*/
void addIntoUnDefineGoto(ListInstruction addr) {
    log_trace("addIntoUnDefineGoto (ListInstruction %p)", addr)
    CHECKPOINTER(addr);

    addr->cursorCode->unDefineGoto[addr->cursorCode->numberGoto] = addr->cursorCode->numberCode;
    addr->cursorCode->numberCode++;
}

Data getFirstDataCursor(Data cursor) {
    Data found = cursor;
    while (found != NULL && found->previousData != NULL)
        found = found->previousData;
    return found;
}

Code getFirstCodeCursor(Code cursor) {
    Code found = cursor;
    while (found != NULL && found->previousCode != NULL)
        found = found->previousCode;
    return found;
}

int writeToFile(ListInstruction list, FILE *file) {
    Data data = getFirstDataCursor(list->cursorData);
    Code code = getFirstCodeCursor(list->cursorCode);
    CHECK(fprintf(file, ".data\n"));

    while (data->previousData != NULL) {
        for (int i = 0; i < data->numberData; ++i) {
            CHECK(fprintf(file, "%s\n", data->lineData[i]));
        }
        data = data->previousData;
    }

    CHECK(fprintf(file, ".text\n"));

    while (code->previousCode != NULL) {
        for (int i = 0; i < code->numberCode; ++i) {
            CHECK(fprintf(file, "%s\n", code->lineCode[i]));
        }
        code = code->previousCode;
    }

    return RETURN_SUCCESS;
}