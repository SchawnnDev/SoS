#include "../include/listRangeVar.h"

/*!
 * \fn RangeVariable initRangeVariable(int rangeLevel, RangeVariable previousLevel)
 * \brief Fonction qui initialise la structure de portée de variable
*/
RangeVariable initRangeVariable(int rangeLevel, int stack, RangeVariable previousLevel)
{
    log_trace("initRangeVariable (int %d, RangeVariable %p)", rangeLevel,previousLevel)

    if(rangeLevel < 0){
        log_error("rangeLevel : %d : %d",rangeLevel,0)
        perror("initRangeVariable : rangeLevel must be more than 0.");
        return NULL;
    }

    RangeVariable addr;
    CHECKPOINTER(addr = (RangeVariable)malloc(sizeof(struct rangeVariable_t)));
    addr->listIdentifier = initListIdentifier();
    addr->rangeLevel = rangeLevel;
    addr->stack = stack;

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
    addr->cursor = initRangeVariable(0,START_STACK,NULL);

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
 * \fn int addRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui ajoute un niveau de portée à la liste de structure de portée de variable
*/
int addRangeVariable(ListRangeVariable addr)
{
    log_trace("addRangeVariable (ListRangeVariable %p)", addr)
    CHECKPOINTER(addr);

    RangeVariable newCursor = initRangeVariable(addr->cursor->rangeLevel + 1,addr->cursor->stack, addr->cursor);
    addr->cursor->nextLevel = newCursor;
    addr->cursor = newCursor;

    return RETURN_SUCCESS;
}

/*!
 * \fn int deleteRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui supprime un niveau de portée à la liste de structure de portée de variable
*/
int deleteRangeVariable(ListRangeVariable addr)
{
    log_trace("deleteRangeVariable (ListRangeVariable %p)", addr)
    CHECKPOINTER(addr);

    if(addr->cursor->rangeLevel == 0){
        log_error("rangeLevel : %d",addr->cursor->rangeLevel)
        perror("deleteRangeVariable : there is no negative rangeLevel.");
        return RETURN_FAILURE;
    }

    RangeVariable tmp = addr->cursor;
    addr->cursor = tmp->previousLevel;
    addr->cursor->nextLevel = NULL;
    cleanRangeVariable(tmp);

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
    while((tmp != NULL) && (position == NOTFOUND)){
        position = searchIntoListIdentifier(tmp->listIdentifier,name);

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
int addIdentifier(ListRangeVariable addr, char *name, int saveToStack)
{
    log_trace("addIdentifier (ListRangeVariable %p, char* %s)", addr, name)
    CHECKPOINTER(addr);
    CHECKPOINTER(name);

    VariablePosition variablePosition = searchIdentifierPosition(addr,name);

    if(variablePosition->indexIdentifier != NOTFOUND){
        log_error("Identifier found : position : %d",variablePosition->indexIdentifier)
        perror("addIdentifier : can not set add existing identifier.");
        return RETURN_FAILURE;
    }

    return addIntoListIdentifier(addr->cursor->listIdentifier, name, newMemorySlot());
}


/*!
 * \fn int setType(ListRangeVariable addr, char* name, int type)
 * \brief Fonction qui modifie le type de l'identificateur dans la liste des postée de variable
*/
int setType(ListRangeVariable addr, char* name, int type)
{
    log_trace("setType (ListRangeVariable %p, char* %s, int %d)", addr, name, type)
    CHECKPOINTER(addr);
    CHECKPOINTER(name);

    VariablePosition variablePosition = searchIdentifierPosition(addr,name);
    if(variablePosition->rangePosition == NULL){
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
    CHECKPOINTER(addr);
    CHECKPOINTER(name);

    VariablePosition variablePosition = searchIdentifierPosition(addr,name);
    if(variablePosition->rangePosition == NULL){
        return RETURN_FAILURE;
    }
    return setArraySizeOfIdentifier(variablePosition->rangePosition->listIdentifier,
                                                                          variablePosition->indexIdentifier, arraySize);
}

/*!
 * \fn int setValuesFromListTmp(ListRangeVariable addr, char* name, ListTmp addrTmp)
 * \brief Fonction remplie le tableau des valeurs de l'identificateur dans la liste des postée de variable depuis à la liste temporaire
*/
int setValuesFromListTmp(ListRangeVariable addr, char* name, ListTmp addrTmp)
{
    log_trace("setValuesFromListTmp (ListRangeVariable %p, char* %s, ListTmp %p)", addr, name, addrTmp)
    CHECKPOINTER(addr);
    CHECKPOINTER(name);
    CHECKPOINTER(addrTmp);

    VariablePosition variablePosition = searchIdentifierPosition(addr,name);
    if(variablePosition->rangePosition == NULL){
        return RETURN_FAILURE;
    }
    return setValuesOfIdentifierFromListTmp(variablePosition->rangePosition->listIdentifier,
                                            variablePosition->indexIdentifier, addrTmp);
}

/*!
 * \fn int getValuesFromIdentifier(ListRangeVariable addr, char* name, ListTmp addrTmp)
 * \brief Fonction remplie liste temporaire avec la liste des identificateurs depuis la liste des postée de variable
*/
int getValuesFromIdentifier(ListRangeVariable addr, char* name, ListTmp addrTmp)
{
    log_trace("getValuesFromIdentifier (ListRangeVariable %p, char* %s, ListTmp %p)", addr, name, addrTmp)
    CHECKPOINTER(addr);
    CHECKPOINTER(name);
    CHECKPOINTER(addrTmp);

    VariablePosition variablePosition = searchIdentifierPosition(addr,name);
    if(variablePosition->rangePosition == NULL){
        return RETURN_FAILURE;
    }
    return getValuesFromIdentifierToListTmp(variablePosition->rangePosition->listIdentifier,
                                            variablePosition->indexIdentifier, -1, addrTmp);
}

/*!
 * \fn int getValuesFromIdentifier(ListRangeVariable addr, char* name, ListTmp addrTmp)
 * \brief Fonction remplie liste temporaire avec la liste des identificateurs depuis la liste des postée de variable
*/
int getValuesFromIdentifierWithIndex(ListRangeVariable addr, char* name,int index, ListTmp addrTmp)
{
    log_trace("getValuesFromIdentifier (ListRangeVariable %p, char* %s, int %d, ListTmp %p)", addr, name, index, addrTmp)
    CHECKPOINTER(addr);
    CHECKPOINTER(name);
    CHECKPOINTER(addrTmp);

    VariablePosition variablePosition = searchIdentifierPosition(addr,name);
    if(variablePosition->rangePosition == NULL){
        return RETURN_FAILURE;
    }
    return getValuesFromIdentifierToListTmp(variablePosition->rangePosition->listIdentifier,
                                            variablePosition->indexIdentifier, index, addrTmp);
}

/*!
 * \fn IdentifierOrder initIdentifierOrder(IdentifierOrder previousLevel, char* name)
 * \brief Fonction qui initialise la structure d'ordre d'apparition des identificateurs
*/
IdentifierOrder initIdentifierOrder(IdentifierOrder previousIdentifier, char* name)
{
    log_trace("initIdentifierOrder(IdentifierOrder %p, char* %s)", previousIdentifier, name)

    if(strcmp(name, "") == 0){
        log_error("name : %s, notEmpty",name)
        perror("initIdentifier : name is Empty or identifier can't be empty.");
        return NULL;
    }

    IdentifierOrder addr;
    CHECKPOINTER(addr = (IdentifierOrder)malloc(sizeof(struct identifierOrder_t)));
    addr->previousIdentifier = previousIdentifier;
    ulong size = strlen( name ) + 1;
    CHECKPOINTER(addr->name = (char*)malloc(sizeof(char) * size));
    CHECKPOINTER(strcpy(addr->name,name));
    addr->type = UNSET;
    addr->index = -1;

    return addr;
}

/*!
 * \fn void cleanIdentifierOrder(IdentifierOrder addr)
 * \brief Fonction qui libère la mémoire d'une structure d'ordre d'apparition des identificateurs
*/
void cleanIdentifierOrder(IdentifierOrder addr)
{
    log_trace("cleanIdentifierOrder (IdentifierOrder %p)",addr)
    CHECKPOINTER(addr);

    free(addr->name);
    free(addr);
}

/*!
 * \fn ListRangeVariable initListRangeVariable()
 * \brief Fonction qui initialise la liste de structure d'ordre d'apparition des identificateurs
*/
ListIdentifierOrder initListIdentifierOrder()
{
    log_trace("initListIdentifierOrder (void)")

    ListIdentifierOrder addr;
    CHECKPOINTER(addr = (ListIdentifierOrder)malloc(sizeof(listIdentifierOrder_t)));
    addr->cursor = NULL;

    return addr;
}

/*!
 * \fn void cleanListRangeVariable(ListRangeVariable addr)
 * \brief Fonction qui libère la mémoire d'une liste de structure d'ordre d'apparition des identificateurs
*/
void cleanListIdentifierOrder(ListIdentifierOrder addr)
{
    log_trace("cleanListIdentifierOrder(ListIdentifierOrder %p)", addr)
    CHECKPOINTER(addr);

    IdentifierOrder tmp, addrToFree = addr->cursor;
    while(addrToFree != NULL){
        tmp = addrToFree->previousIdentifier;
        cleanIdentifierOrder(addrToFree);
        addrToFree = tmp;
    }

    free(addr);
}

/*!
 * \fn int addIdentifierOrder(ListIdentifierOrder addr, char * name)
 * \brief Fonction qui ajoute un niveau de portée à la liste d'ordre d'apparition des identificateurs
*/
void addIdentifierOrder(ListIdentifierOrder addr, char * name)
{
    log_trace("addIdentifierOrder(ListIdentifierOrder %p, char * %s)", addr,name)
    CHECKPOINTER(addr);

    addr->cursor = initIdentifierOrder(addr->cursor, name);
}

/*!
 * \fn int setTypeIdentifierOrder(ListIdentifierOrder addr, int type)
 * \brief Fonction qui modifie le type du dernier identificateur de la liste de structure d'ordre d'apparition des identificateurs
*/
int setTypeIdentifierOrder(ListIdentifierOrder addr, int type)
{
    log_trace("setTypeIdentifierOrder(ListIdentifierOrder %p, int %d)", addr,type)
    CHECKPOINTER(addr);

    if((type <= UNSET ) || (type >= MAXTYPEVALUE)){
        log_error("type : %d : %d > type < %d",type,UNSET,MAXTYPEVALUE)
        perror("setTypeIdentifierOrder : this type value doesn't exist.");
        return RETURN_FAILURE;
    }

    if(addr->cursor != NULL)
        addr->cursor->type = type;

    return RETURN_SUCCESS;
}


/*!
 * \fn int setIndexIdentifierOrder(ListIdentifierOrder addr, int index)
 * \brief Fonction qui modifie l'index pour l'affectation d'un tableau du dernier identificateur de la liste de structure d'ordre d'apparition des identificateurs
*/
int setIndexIdentifierOrder(ListIdentifierOrder addr, int index)
{
    log_trace("setIndexIdentifierOrder(ListIdentifierOrder %p, int %d)", addr,index)
    CHECKPOINTER(addr);

    if(index <= UNSET){
        log_error("index : %d : %d > index",index,UNSET)
        perror("setIndexIdentifierOrder : this index isn't accept.");
        return RETURN_FAILURE;
    }

    addr->cursor->index = index;

    return RETURN_SUCCESS;
}

/*!
 * \fn int increaseStackSize(ListRangeVariable addr, int amount)
 * \brief Fonction qui modifie la stack et renvoie la valeur précédante
*/
int increaseStackSize(ListRangeVariable addr, int amount)
{
    int stack = addr->cursor->stack;
    addr->cursor->stack += amount;
    return stack;
}

/*!
 * \fn int increaseStackSize(ListRangeVariable addr, int amount)
 * \brief Fonction qui recupère la valeur de la stack
*/
int getStack(ListRangeVariable addr)
{
   return addr->cursor->stack;
}

/*!
 * \fn int deleteIdentifierOrder(ListIdentifierOrder addr)
 * \brief Fonction qui supprime un niveau de portée à la liste de structure d'ordre d'apparition des identificateurs
*/
int deleteIdentifierOrder(ListIdentifierOrder addr)
{
    log_trace("deleteIdentifierOrder(ListIdentifierOrder %p)", addr)
    CHECKPOINTER(addr);

    if(addr->cursor == NULL){
        log_error("rangeLevel : %p",addr->cursor)
        perror("deleteRangeVariable : there is no negative rangeLevel.");
        return RETURN_FAILURE;
    }

    IdentifierOrder tmp = addr->cursor;
    addr->cursor = tmp->previousIdentifier;
    cleanIdentifierOrder(tmp);

    return RETURN_SUCCESS;
}

/*!
 * \fn int setOffset(ListIdentifier addr, int position, int offset)
 * \brief Fonction qui modifie l'offset de l'identificateur
*/
int setOffset(ListRangeVariable addr, char* name, MemorySpace offset)
{
    log_trace("setOffset (ListRangeVariable %p, char* %s, int %d)", addr, name, offset)
    CHECKPOINTER(addr);
    CHECKPOINTER(name);

    VariablePosition variablePosition = searchIdentifierPosition(addr,name);
    if(variablePosition->rangePosition == NULL){
        return RETURN_FAILURE;
    }
    return setOffsetOfIdentifier(variablePosition->rangePosition->listIdentifier,
                                            variablePosition->indexIdentifier, offset);
}

/*!
 * \fn int getOffset(ListIdentifier addr, int position, ListTmp listTmp)
 * \brief Fonction qui récupère l'offset de l'identificateur
*/
/*int getOffset(ListRangeVariable addr, char* name, ListTmp listTmp)
{
    log_trace("getOffset (ListRangeVariable %p, char* %s)", addr, name)
    CHECKPOINTER(addr);
    CHECKPOINTER(name);
    CHECKPOINTER(listTmp);

    VariablePosition variablePosition = searchIdentifierPosition(addr,name);
    if(variablePosition->rangePosition == NULL){
        return RETURN_FAILURE;
    }

    char* offset;
    CHECKPOINTER(offset = (char*)malloc(sizeof(char) * SIZE_INT_STR));
    CHECK(sprintf(offset,"%d", (addr->cursor->stack - ADDR_STACK_SIZE - getOffsetOfIdentifier(
            variablePosition->rangePosition->listIdentifier,variablePosition->indexIdentifier))));
    return addIntoListTmpWithType(listTmp,offset, TYPE_STACK);
}*/


/*!
 * \fn int printIdentifierFromListRange(ListRangeVariable addr,char* name)
 * \brief Fonction affiche l'état actuelle d'un identificateur
*/
int printIdentifierFromListRange(ListRangeVariable addr,char* name)
{
    log_trace("printIdentifierFromListRange (ListRangeVariable %p, char* %s)", addr, name)
    CHECKPOINTER(addr);
    CHECKPOINTER(name);

    VariablePosition variablePosition = searchIdentifierPosition(addr,name);
    if(variablePosition->rangePosition == NULL){
        return RETURN_FAILURE;
    }
    return printIdentifier(variablePosition->rangePosition->listIdentifier,variablePosition->indexIdentifier);
}