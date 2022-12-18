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