#include "../include/listRangeVar.h"

/*!
 * \fn RangeVariable initRangeVariable(int rangeLevel, RangeVariable previousLevel)
 * \brief Fonction qui initialise la structure de portée de variable
*/
RangeVariable initRangeVariable(int rangeLevel, RangeVariable previousLevel)
{
    log_trace("initListTmp (int %d)", rangeLevel)

    if(rangeLevel < 0){
        log_error("rangeLevel : %d : %d",rangeLevel,0)
        perror("initListTmp : rangeLevel must be more than 0.");
        exit(EXIT_FAILURE);
    }

    RangeVariable addr;
    CHECKPOINTER(addr = (RangeVariable)malloc(sizeof(struct rangeVariable_t)));
    addr->listIdentifier = initListIdentifier();
    addr->rangeLevel = rangeLevel;

    addr->previousLevel = previousLevel;
    addr->nextLevel = NULL;

    return addr;
}

/*!
 * \fn void cleanRangeVariable(RangeVariable addr)
 * \brief Fonction qui libère la mémoire d'une structure de portée de variable
*/
void cleanRangeVariable(RangeVariable addr)
{
    log_trace("cleanRangeVariable (RangeVariable %p)", addr)
    CHECKPOINTER(addr);

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
    CHECKPOINTER(addr = (ListRangeVariable)malloc(sizeof(listRangeVariable_t)));
    addr->cursor = initRangeVariable(0,NULL);

    return addr;
}

/*!
 * \fn void cleanListRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui libère la mémoire d'une list de structure de portée de variable
*/
void cleanListRangeVariable(ListRangeVariable addr)
{
    log_trace("cleanListRangeVariable (ListRangeVariable %p)", addr)
    CHECKPOINTER(addr);

    RangeVariable tmp, addrToFree = addr->cursor;
    while(addrToFree != NULL){
        tmp = addrToFree->previousLevel;
        cleanRangeVariable(addrToFree);
        addrToFree = tmp;
    }

    free(addr);
}

/*!
 * \fn ListRangeVariable addRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui ajoute un niveau de portée à la liste de structure de portée de variable
*/
ListRangeVariable addRangeVariable(ListRangeVariable addr)
{
    log_trace("addRangeVariable (ListRangeVariable %p)", addr)
    CHECKPOINTER(addr);

    RangeVariable newCursor = initRangeVariable(addr->cursor->rangeLevel + 1, addr->cursor);
    addr->cursor->nextLevel = newCursor;
    addr->cursor = newCursor;

    return addr;
}

/*!
 * \fn ListRangeVariable deleteRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui supprime un niveau de portée à la liste de structure de portée de variable
*/
ListRangeVariable deleteRangeVariable(ListRangeVariable addr)
{
    log_trace("deleteRangeVariable (ListRangeVariable %p)", addr)
    CHECKPOINTER(addr);

    if(addr->cursor->rangeLevel == 0){
        log_error("rangeLevel : %d",addr->cursor->rangeLevel)
        perror("deleteRangeVariable : there is no negative rangeLevel.");
        exit(EXIT_FAILURE);
    }

    RangeVariable tmp = addr->cursor;
    addr->cursor = tmp->previousLevel;
    addr->cursor->nextLevel = NULL;
    cleanRangeVariable(tmp);

    return addr;
}

/*!
 * \fn VariablePosition initVariablePosition(RangeVariable rangePosition, int indexIdentifier)
 * \brief Fonction qui initialise la structure de position d'identificateur
*/
VariablePosition initVariablePosition(RangeVariable rangePosition, int indexIdentifier)
{
    log_trace("initVariablePosition (RangeVariable %p, int %d)", rangePosition, indexIdentifier)

    VariablePosition addr;
    CHECKPOINTER(addr = (VariablePosition)malloc(sizeof(variablePosition_t)));
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
    CHECKPOINTER(addr);
    CHECKPOINTER(name);

    int position = NOTFOUND;
    RangeVariable tmp = addr->cursor;
    while(tmp != NULL){
        position = searchIntoListIdentifier(tmp->listIdentifier,name);

        if(position == NOTFOUND){
            tmp = tmp->previousLevel;
        }
    }

    return initVariablePosition(tmp,position);
}

/*!
 * \fn ListRangeVariable addIdentifier(ListRangeVariable addr, char* name)
 * \brief Fonction qui ajoute unidentificateur dans la liste des postée de variable
*/
ListRangeVariable addIdentifier(ListRangeVariable addr, char* name)
{
    log_trace("addIdentifier (ListRangeVariable %p, char* %s)", addr, name)
    CHECKPOINTER(addr);
    CHECKPOINTER(name);

    VariablePosition variablePosition = searchIdentifierPosition(addr,name);

    if(variablePosition->indexIdentifier != NOTFOUND){
        log_error("Identifier found : position : %d",variablePosition->indexIdentifier)
        perror("addIdentifier : can not set add existing identifier.");
        exit(EXIT_FAILURE);
    }

    addr->cursor->listIdentifier = addIntoListIdentifier(addr->cursor->listIdentifier, name);

    return addr;
}