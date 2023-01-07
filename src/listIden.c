#include "../include/listIden.h"

/*!
 * \fn ListIdentifier initListIdentifier()
 * \brief Fonction qui initialise la structure des Identificateurs
*/
ListIdentifier initListIdentifier()
{
    log_trace("initListIdentifier (void)")

    ListIdentifier addr;
    CHECKPOINTER(addr = (ListIdentifier)malloc(sizeof(listIdentifier_t)))
    CHECK_ERROR_RETURN(NULL)
    addr->numberIdentifiers = 0;
    return addr;
}

/*!
 * \fn void cleanListIdentifier(ListIdentifier addr)
 * \brief Fonction qui libère la mémoire de la structure des Identificateurs
*/
void cleanListIdentifier(ListIdentifier addr)
{
    log_trace("cleanListIdentifier (ListIdentifier %p)",addr)
    CHECKPOINTER(addr)
    CHECK_ERROR_NORETURN()

    int index;
    int size = addr->numberIdentifiers;
    for(index = 0; index < size; index++){
        cleanIdentifier(addr->Identifiers[index]);
    }

    free(addr);
}

/*!
 * \fn Identifier initIdentifier(char* name)
 * \brief Fonction qui crée un Identifier
*/
Identifier initIdentifier(char* name)
{
    log_trace("initIdentifier (char* %s)",name)

    if(strcmp(name, "") == 0){
        log_error("name : %s, notEmpty",name)
        perror("initIdentifier : name is Empty or identifier can't be empty.");
        return NULL;
    }

    Identifier addr;
    CHECKPOINTER(addr = (Identifier)malloc(sizeof(identifier_t)));
    ulong size = strlen( name ) + 1;
    CHECKPOINTER(addr->name = (char*)malloc(sizeof(char) * size));
    CHECKPOINTER(strcpy(addr->name,name));
    addr->memory = NULL;
    addr->type = UNSET;
    addr->arraySize = 1;

    return addr;
}

/*!
 * \fn void cleanIdentifier(Identifier addr)
 * \brief Fonction qui libère la mémoire de la structure Identificateur
*/
void cleanIdentifier(Identifier addr)
{
    log_trace("cleanIdentifier (Identifier %p)",addr)
    CHECKPOINTER(addr)
    CHECK_ERROR_NORETURN()

    free(addr);
}

/*!
 * \fn int searchIntoListIdentifier(ListIdentifier addr, char* name)
 * \brief Fonction qui recherche la position de l'Identifier dans la liste des identificateurs
*/
int searchIntoListIdentifier(ListIdentifier addr, char* name)
{
    log_trace("searchIntoListIdentifier (ListIdentifier %p, char* %s)",addr,name)

    int position = NOTFOUND;
    CHECKPOINTER(addr);
    CHECKPOINTER(name);
    int size = addr->numberIdentifiers;

    int index;
    for(index = 0; (index < size) && (position == NOTFOUND); index++)
    {
        if(strcmp(name, addr->Identifiers[index]->name) == 0){
            log_info("Identifier found : name : %s, position : %d",name,index)
            position = index;
        }
    }

    return position;
}

/*!
 * \fn int addIntoListIdentifier(ListIdentifier addr, char* name, char* value)
 * \brief Fonction qui crée un Identifier et l'ajoute en fin de la liste des identificateurs
*/
int addIntoListIdentifier(ListIdentifier addr, char* name, MemorySlot offset)
{
    log_trace("addIntoListIdentifier (ListIdentifier %p, char* %s)",addr,name)
    CHECKPOINTER(addr);
    CHECKPOINTER(name);
    CHECKPOINTER(offset);

    if(addr->numberIdentifiers == IDEN_MAX){
        log_error("numberIdentifiers : %d, %d",addr->numberIdentifiers,IDEN_MAX)
        perror("addIntoListIdentifier : to many Identifier.");
        return RETURN_FAILURE;
    }

    Identifier identifier = initIdentifier(name);
    if(identifier == NULL){
        log_error("identifier : %p, name %s",identifier,name)
        perror("addIntoListIdentifier : identifier isn't well initialized.");
        return RETURN_FAILURE;
    }
    offset->temp = false;
    addr->Identifiers[addr->numberIdentifiers] = identifier;
    addr->Identifiers[addr->numberIdentifiers]->memory = offset;
    addr->numberIdentifiers++;

    return RETURN_SUCCESS;
}

/*!
 * \fn int setTypeOfIdentifier(ListIdentifier addr, int position, int type)
 * \brief Fonction qui modifie le type de l'identificateur
*/
int setTypeOfIdentifier(ListIdentifier addr, int position,int type)
{
    log_trace("setTypeOfIdentifier (ListIdentifier %p, int %d,int %d)",addr,position,type)
    CHECKPOINTER(addr);

    if((type <= UNSET ) || (type >= MAXTYPEVALUE)){
        log_error("type : %d : %d > type < %d",type,UNSET,MAXTYPEVALUE)
        perror("setTypeOfIdentifier : this type value doesn't exist.");
        return RETURN_FAILURE;
    }

    if((position <= NOTFOUND) || (position >= addr->numberIdentifiers)){
        log_error("Position out of range : position : %d",position)
        perror("setTypeOfIdentifier : can not set type of non-existent identifier.");
        return RETURN_FAILURE;
    }

    addr->Identifiers[position]->type = type;

    return RETURN_SUCCESS;
}

/*!
 * \fn ListIdentifier setTypeOfIdentifier(ListIdentifier addr, int position, int type)
 * \brief Fonction qui modifie le type de l'identificateur
*/
int setArraySizeOfIdentifier(ListIdentifier addr, int position, int arraySize)
{
    log_trace("setArraySizeOfIdentifier (ListIdentifier %p, int %d,int %d)",addr,position,arraySize)
    CHECKPOINTER(addr);

    if(arraySize < 1){
        log_error("arraySize : %d : %d",arraySize,1)
        perror("setTypeOfIdentifier : arraySize must be more than 1.");
        return RETURN_FAILURE;
    }

    if((position <= NOTFOUND) || (position >= addr->numberIdentifiers)){
        log_error("Position out of range : position : %d",position)
        perror("setArraySizeOfIdentifier : can not set type of non-existent identifier.");
        return RETURN_FAILURE;
    }

    addr->Identifiers[position]->arraySize = arraySize;

    return RETURN_SUCCESS;
}

/*!
 * \fn int setOffsetOfIdentifier(ListIdentifier addr, int position, char* reg)
 * \brief Fonction modifier le registre de l'identificateur
*/
int setOffsetOfIdentifier(ListIdentifier addr, int position, MemorySlot offset)
{
    log_trace("setOffsetOfIdentifier (ListIdentifier %p, int %d, int %d)",addr,position,offset)
    CHECKPOINTER(addr);
    CHECKPOINTER(offset);

    if((position <= NOTFOUND) || (position >= addr->numberIdentifiers)){
        log_error("Position out of range : position : %d",position)
        perror("setOffsetOfIdentifier : can not set the offset of non-existent identifier.");
        return RETURN_FAILURE;
    }

    addr->Identifiers[position]->memory = offset;

    return RETURN_SUCCESS;
}

/*!
 * \fn int getOffsetOfIdentifier(ListIdentifier addr, int position)
 * \brief Fonction recupérer l'offset de l'identificateur
*/
MemorySlot getOffsetOfIdentifier(ListIdentifier addr, int position)
{
    log_trace("getOffsetOfIdentifier (ListIdentifier %p, int %d)", addr, position)
    CHECKPOINTER(addr);

    if((position <= NOTFOUND) || (position >= addr->numberIdentifiers)){
        log_error("Position out of range : position : %d",position)
        perror("getOffsetOfIdentifier : can not get offset of non-existent identifier.");
        return NULL;
    }

    return addr->Identifiers[position]->memory;
}

/*!
 * \fn int printIdentifier(ListIdentifier addr,int position)
 * \brief Fonction affiche l'état actuelle d'un identificateur
*/
int printIdentifier(ListIdentifier addr,int position)
{
    log_trace("printIdentifier (ListIdentifier %p, int %d)",addr,position)
    CHECKPOINTER(addr);

    if((position <= NOTFOUND) || (position >= addr->numberIdentifiers)){
        log_error("Position out of range : position : %d",position)
        perror("printIdentifier : can not print values of non-existent identifier.");
        return RETURN_FAILURE;
    }

    printf("position : %d, name %s\n",position,addr->Identifiers[position]->name);

    return RETURN_SUCCESS;
}

/*!
 * \fn void printListIdentifier(ListIdentifier addr)
 * \brief Fonction affiche l'état actuelle de la liste des identificateurs
*/
void printListIdentifier(ListIdentifier addr)
{
    log_trace("printListIdentifier (ListIdentifier %p)",addr)
    CHECKPOINTER(addr);

    int index;
    int size = addr->numberIdentifiers;

    for(index = 0; index < size; index++){
        printIdentifier(addr,index);
    }
}
