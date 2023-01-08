#include "listRangeVar.h"

/*!
 * \fn RangeVariable initRangeVariable(int rangeLevel, RangeVariable previousLevel)
 * \brief Fonction qui initialise la structure de portée de variable
*/
RangeVariable
initRangeVariable(int rangeLevel, int blockType, RangeVariable previousLevel)
{
    log_trace("initRangeVariable (int %d, RangeVariable %p)", rangeLevel,previousLevel)

    if(rangeLevel < 0){
        log_error("rangeLevel : %d : %d",rangeLevel,0)
        perror("initRangeVariable : rangeLevel must be more than 0.");
        setErrorFailure();
        CHECK_ERROR_RETURN(NULL)
    }

    RangeVariable addr;
    CHECKPOINTER(addr = (RangeVariable)malloc(sizeof(struct rangeVariable_t)))
    CHECK_ERROR_RETURN(NULL)

    addr->listIdentifier = initListIdentifier();
    CHECK_ERROR_RETURN(NULL)
    addr->rangeLevel = rangeLevel;
    addr->blockType = blockType;

    addr->previousLevel = previousLevel;
    addr->nextLevel = NULL;

    addr->currentFunction = NULL;

    return addr;
}

/*!
 * \fn void cleanRangeVariable(RangeVariable addr)
 * \brief Fonction qui libère la mémoire d'une structure de portée de variable
*/
void cleanRangeVariable(RangeVariable addr)
{
    log_trace("cleanRangeVariable (RangeVariable %p)", addr)
    CHECKPOINTER(addr)
    CHECK_ERROR_NORETURN()

    cleanListIdentifier(addr->listIdentifier);
    free(addr);
}

/*!
 * \fn ListRangeVariable initListRangeVariable()
 * \brief Fonction qui initialise la liste de structure de portée de variable
*/
ListRangeVariable initListRangeVariable()
{
    log_trace("initListRangeVariable (void)")

    ListRangeVariable addr;
    CHECKPOINTER(addr = (ListRangeVariable)malloc(sizeof(listRangeVariable_t)))
    CHECK_ERROR_RETURN(NULL)
    RangeVariable rangeAddr = initRangeVariable(0, BLOCK_MAIN, NULL);
    CHECK_ERROR_RETURN(NULL)
    addr->cursor = rangeAddr;
    addr->cursorGlobal = rangeAddr;

    rangeAddr->memorySlot = NULL;
    CHECKPOINTER(rangeAddr->memoryCurrentStackOffset = malloc(sizeof(int)))
    CHECK_ERROR_RETURN(NULL)
    *rangeAddr->memoryCurrentStackOffset = 0;

    return addr;
}

/*!
 * \fn void cleanListRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui libère la mémoire d'une list de structure de portée de variable
*/
void cleanListRangeVariable(ListRangeVariable addr)
{
    log_trace("cleanListRangeVariable (ListRangeVariable %p)", addr)
    CHECKPOINTER(addr)
    CHECK_ERROR_NORETURN()

    RangeVariable tmp, addrToFree = addr->cursor;
    while(addrToFree != NULL){
        tmp = addrToFree->previousLevel;
        cleanRangeVariable(addrToFree);
        addrToFree = tmp;
    }

    free(addr);
}

/*!
 * \fn int increaseGlobalRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui augmente la taille de la liste de portée de vaiable global en ajoutant un nouveau bloque
*/
int increaseGlobalRangeVariable(ListRangeVariable addr)
{
    log_trace("increaseGlobalRangeVariable (ListRangeVariable %p)", addr)
    CHECKPOINTER(addr)
    CHECK_ERROR_RETURN(RETURN_FAILURE)
    CHECKPOINTER(addr->cursorGlobal)
    CHECKPOINTER(addr->cursor)
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    RangeVariable newCursor = initRangeVariable(0, BLOCK_MAIN,
                                                addr->cursorGlobal);
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    if(addr->cursorGlobal->nextLevel != NULL){
        newCursor->nextLevel = addr->cursorGlobal->nextLevel;
    } else {
        addr->cursor = newCursor;
    }

    addr->cursorGlobal->nextLevel = newCursor;
    addr->cursorGlobal = newCursor;

    return RETURN_SUCCESS;
}

/*!
 * \fn int addRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui ajoute un niveau de portée à la liste de structure de portée de variable
*/
int addRangeVariable(ListRangeVariable addr, int blockType)
{
    log_trace("addRangeVariable (ListRangeVariable %p)", addr)
    CHECKPOINTER(addr)
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    RangeVariable newCursor = initRangeVariable(addr->cursor->rangeLevel + 1,
                                                blockType, addr->cursor);
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    // Change here the memory
    newCursor->memorySlot = NULL;
    CHECKPOINTER(newCursor->memoryCurrentStackOffset = malloc(sizeof(int)))
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    addr->cursor->nextLevel = newCursor;
    addr->cursor = newCursor;

    asm_writeRegistersToStack();
    // asm_appendInternalOffset(ASM_VAR_REGISTERS_CACHE_COUNT); // +1 is $ra
    asm_code_printf("\tli $s7, 0\n")

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

/*!
 * \fn int deleteRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui supprime un niveau de portée à la liste de structure de portée de variable
*/
int deleteRangeVariable(ListRangeVariable addr)
{
    log_trace("deleteRangeVariable (ListRangeVariable %p)", addr)
    CHECKPOINTER(addr)
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    if(addr->cursor->rangeLevel == 0){
        log_error("rangeLevel : %d",addr->cursor->rangeLevel)
        perror("deleteRangeVariable : there is no negative rangeLevel.");
        setErrorFailure();
        return RETURN_FAILURE;
    }

    RangeVariable tmp = addr->cursor;

    // Change here the memory
    destroyMemorySlot(tmp->memorySlot);
    free(tmp->memoryCurrentStackOffset);

    addr->cursor = tmp->previousLevel;
    addr->cursor->nextLevel = NULL;
    cleanRangeVariable(tmp);
    //asm_subtractInternalOffset(ASM_VAR_REGISTERS_CACHE_COUNT); // +1 is $ra
    asm_loadRegistersFromStack();

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return RETURN_SUCCESS;
}

/*!
 * \fn VariablePosition initVariablePosition(RangeVariable rangePosition, int indexIdentifier)
 * \brief Fonction qui initialise la structure de position d'identificateur
*/
VariablePosition initVariablePosition(RangeVariable rangePosition, int indexIdentifier)
{
    log_trace("initVariablePosition (RangeVariable %p, int %d)", rangePosition, indexIdentifier)

    VariablePosition addr;
    CHECKPOINTER(addr = (VariablePosition)malloc(sizeof(variablePosition_t)))
    CHECK_ERROR_RETURN(NULL)
    addr->rangePosition = rangePosition;
    addr->indexIdentifier = indexIdentifier;

    return addr;
}

/*!
 * \fn void cleanVariablePosition(VariablePosition addr)
 * \brief Fonction qui libère la mémoire d'une structure de position d'identificateur
*/
void cleanVariablePosition(VariablePosition addr)
{
    log_trace("cleanVariablePosition (VariablePosition %p)", addr)
    free(addr);
}

/*!
 * \fn VariablePosition searchIdentifierPosition(ListRangeVariable addr, char* name)
 * \brief Fonction qui cherche la position de l'identificateur dans la liste des postée de variable
*/
VariablePosition searchIdentifierPosition(ListRangeVariable addr, char* name)
{
    log_trace("searchIdentifierPosition (ListRangeVariable %p, char* %s)", addr, name)
    CHECKPOINTER(addr)
    CHECKPOINTER(name)
    CHECK_ERROR_RETURN(NULL)

    int position = NOTFOUND;
    RangeVariable tmp = addr->cursor;
    while((tmp != NULL) && (position == NOTFOUND)){
        position = searchIntoListIdentifier(tmp->listIdentifier,name);
        CHECK_ERROR_RETURN(NULL)

        if(position == NOTFOUND){
            tmp = tmp->previousLevel;
        }
    }

    return initVariablePosition(tmp,position);
}

/*!
 * \fn int addIdentifier(ListRangeVariable addr, char* name)
 * \brief Fonction qui ajoute unidentificateur dans la liste des postée de variable
*/
int addIdentifier(ListRangeVariable addr, char *name)
{
    log_trace("addIdentifier (ListRangeVariable %p, char* %s)", addr, name)
    CHECKPOINTER(addr)
    CHECK_ERROR_RETURN(RETURN_FAILURE)
    CHECKPOINTER(addr->cursorGlobal)
    CHECKPOINTER(name)
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    VariablePosition variablePosition = searchIdentifierPosition(addr,name);
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    if(variablePosition->indexIdentifier != NOTFOUND){
        log_info("Identifier found : position : %d",variablePosition->indexIdentifier)
        return RETURN_FAILURE;
    }

    if(addr->cursorGlobal->listIdentifier->numberIdentifiers >= IDEN_MAX){
        log_info("No more place into global range variable, so auto increase is called")
        increaseGlobalRangeVariable(addr);
        CHECK_ERROR_RETURN(RETURN_FAILURE)
    }

    MemorySlot space;
    CHECKPOINTER(space = malloc(sizeof(struct memory_space_t)))
    CHECK_ERROR_RETURN(RETURN_FAILURE)
    space->used = false;
    space->offset = -1;
    space->next = NULL;
    // len(var_ + NUL char) = 5
    size_t len = strlen(name) + 5;
    CHECKPOINTER(space->label = malloc(len))
    CHECK_ERROR_RETURN(RETURN_FAILURE)
    snprintf(space->label, len, "var_%s", name);

    asm_data_printf("\t%s: .word 0\n", space->label)

    CHECK_ERROR_RETURN(RETURN_FAILURE)
    return addIntoListIdentifier(addr->cursorGlobal->listIdentifier, name, space);
}

/*!
 * \fn int addLocalIdentifier(ListRangeVariable addr, char* name)
 * \brief Fonction qui ajoute un identificateur dans la liste des postée de variable
*/
int addLocalIdentifier(ListRangeVariable addr, char *name)
{
    log_trace("addLocalIdentifier (ListRangeVariable %p, char* %s)", addr, name)
    CHECKPOINTER(addr)
    CHECK_ERROR_RETURN(RETURN_FAILURE)
    CHECKPOINTER(addr->cursor)
    CHECKPOINTER(name)
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    VariablePosition variablePosition = searchIdentifierPosition(addr,name);
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    if(variablePosition->indexIdentifier != NOTFOUND){
        log_info("Identifier found : position : %d",variablePosition->indexIdentifier)
        return RETURN_FAILURE;
    }

    if(addr->cursor->rangeLevel == 0){
        log_error("You can't add local variable into a global context : rangeLevel %d", addr->cursor->rangeLevel)
        perror("addLocalIdentifier : you try to add into global context.");
        setErrorFailure();
        return RETURN_FAILURE;
    }

    // TODO: impl offset management

    return addIntoListIdentifier(addr->cursor->listIdentifier, name,reserveBlockMemorySlot(addr));
}

/*!
 * \fn int setType(ListRangeVariable addr, char* name, int type)
 * \brief Fonction qui modifie le type de l'identificateur dans la liste des postée de variable
*/
int setType(ListRangeVariable addr, char* name, int type)
{
    log_trace("setType (ListRangeVariable %p, char* %s, int %d)", addr, name, type)
    CHECKPOINTER(addr)
    CHECKPOINTER(name)
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    VariablePosition variablePosition = searchIdentifierPosition(addr,name);
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    if(variablePosition->rangePosition == NULL){
        setErrorFailure();
        return RETURN_FAILURE;
    }
    return setTypeOfIdentifier(variablePosition->rangePosition->listIdentifier,
                                                                          variablePosition->indexIdentifier, type);
}

/*!
 * \fn int setArraySize(ListRangeVariable addr, char* name, int arraySize)
 * \brief Fonction qui modifie le type de l'identificateur dans la liste des postée de variable
*/
int setArraySize(ListRangeVariable addr, char* name, int arraySize)
{
    log_trace("setArraySize (ListRangeVariable %p, char* %s, int %d)", addr, name, arraySize)
    CHECKPOINTER(addr)
    CHECKPOINTER(name)
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    VariablePosition variablePosition = searchIdentifierPosition(addr,name);
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    if(variablePosition->rangePosition == NULL){
        setErrorFailure();
        return RETURN_FAILURE;
    }
    return setArraySizeOfIdentifier(variablePosition->rangePosition->listIdentifier,
                                                                          variablePosition->indexIdentifier, arraySize);
}

/*!
 * \fn int printIdentifierFromListRange(ListRangeVariable addr,char* name)
 * \brief Fonction affiche l'état actuelle d'un identificateur
*/
int printIdentifierFromListRange(ListRangeVariable addr,char* name)
{
    log_trace("printIdentifierFromListRange (ListRangeVariable %p, char* %s)", addr, name)
    CHECKPOINTER(addr)
    CHECKPOINTER(name)
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    VariablePosition variablePosition = searchIdentifierPosition(addr,name);
    CHECK_ERROR_RETURN(RETURN_FAILURE)

    if(variablePosition->rangePosition == NULL){
        setErrorFailure();
        return RETURN_FAILURE;
    }
    return printIdentifier(variablePosition->rangePosition->listIdentifier,variablePosition->indexIdentifier);
}

/**
 *
 * @param addr
 * @return
 */
MemorySlot reserveBlockMemorySlot(ListRangeVariable addr)
{
    MemorySlot mem = reserveMemorySlot(addr->cursor->memorySlot,
                                       addr->cursor->memoryCurrentStackOffset);
    CHECK_ERROR_RETURN(NULL)

    if (addr->cursor->memorySlot == NULL)
    {
        addr->cursor->memorySlot = mem;
        return addr->cursor->memorySlot;
    }

    return mem;
}