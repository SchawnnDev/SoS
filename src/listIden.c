#include "../include/listIden.h"

/*!
 * \fn ListIdentifier initListIdentifier()
 * \brief Fonction qui initialise la structure des Identificateurs
*/
ListIdentifier initListIdentifier()
{
    log_trace("initListIdentifier (void)")

    ListIdentifier addr;
    CHECKPOINTER(addr = (ListIdentifier)malloc(sizeof(listIdentifier_t)));
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
    CHECKPOINTER(addr);

    int index;
    int size = addr->numberIdentifiers;
    for(index = 0; index < size; index++){
        cleanIdentifier(addr->Identifiers[index]);
    }

    free(addr->Identifiers);
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
        exit(EXIT_FAILURE);
    }

    Identifier addr;
    CHECKPOINTER(addr = (Identifier)malloc(sizeof(identifier_t)));
    ulong size = strlen( name ) + 1;
    CHECKPOINTER(addr->name = (char*)malloc(sizeof(char) * size));
    CHECKPOINTER(strcpy(addr->name,name));
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
    CHECKPOINTER(addr);

    int index;
    int size = addr->arraySize;
    for(index = 0; index < size; index++){
        free(addr->values[index]);
    }

    free(addr->values);
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
 * \fn ListIdentifier addIntoListIdentifier(ListIdentifier addr, char* name, char* value)
 * \brief Fonction qui crée un Identifier et l'ajoute en fin de la liste des identificateurs
*/
ListIdentifier addIntoListIdentifier(ListIdentifier addr, char* name)
{
    log_trace("addIntoListIdentifier (ListIdentifier %p, char* %s)",addr,name)
    CHECKPOINTER(addr);
    CHECKPOINTER(name);

    if(addr->numberIdentifiers == IDEN_MAX){
        log_error("numberIdentifiers : %d, %d",addr->numberIdentifiers,IDEN_MAX)
        perror("addIntoListIdentifier : to many Identifier.");
        exit(EXIT_FAILURE);
    }

    addr->Identifiers[addr->numberIdentifiers] = initIdentifier(name);
    addr->numberIdentifiers++;

    return addr;
}

/*!
 * \fn ListIdentifier setTypeOfIdentifier(ListIdentifier addr, int position, int type)
 * \brief Fonction qui modifie le type de l'identificateur
*/
ListIdentifier setTypeOfIdentifier(ListIdentifier addr, int position,int type)
{
    log_trace("setTypeOfIdentifier (ListIdentifier %p, int %d,int %d)",addr,position,type)
    CHECKPOINTER(addr);

    if((type < UNSET ) || (type > MAXTYPEVALUE)){
        log_error("type : %d : %d > type < %d",type,UNSET,MAXTYPEVALUE)
        perror("setTypeOfIdentifier : this type value doesn't exist.");
        exit(EXIT_FAILURE);
    }

    if((position == NOTFOUND) && (position >= addr->numberIdentifiers)){
        log_error("Position out of range : position : %d",position)
        perror("setTypeOfIdentifier : can not set type of non-existent identifier.");
        exit(EXIT_FAILURE);
    }

    addr->Identifiers[position]->type = type;

    return addr;
}

/*!
 * \fn ListIdentifier setValuesOfIdentifierFromListTmp(ListIdentifier addr, int position, ListTmp addrTmp)
 * \brief Fonction remplie le tableau des valeurs de l'identificateur grave à la liste temporaire
*/
ListIdentifier setValuesOfIdentifierFromListTmp(ListIdentifier addr, int position, ListTmp addrTmp)
{
    log_trace("setValuesOfIdentifierFromListTmp (ListIdentifier %p, int %d, ListTmp %p)",addr,position,addrTmp)
    CHECKPOINTER(addr);
    CHECKPOINTER(addrTmp);

    if((position == NOTFOUND) && (position >= addr->numberIdentifiers)){
        log_error("Position out of range : position : %d",position)
        perror("setValuesOfIdentifierFromListTmp : can not set type of non-existent identifier.");
        exit(EXIT_FAILURE);
    }

    ulong index;
    ulong size = addr->Identifiers[position]->arraySize;
    if(addr->Identifiers[position]->values != NULL){
        log_info("Identifier will be freed.")

        for(index = 0; index < size; index++){
            free(addr->Identifiers[position]->values[index]);
        }
        free(addr->Identifiers[position]->values);
    }

    char** newAddr;
    CHECKPOINTER(newAddr = (char**)malloc(sizeof(char*) * (addr->Identifiers[position]->arraySize)));
    size = addrTmp->numberValues;
    ulong sizeStr;

    for(index = 0; index < size; index ++){
        sizeStr = strlen( addrTmp->values[index]) + 1;
        CHECKPOINTER(newAddr[index] = (char*)malloc(sizeof(char) * sizeStr));
        CHECKPOINTER(addrTmp->values[index]);
        CHECKPOINTER(strcpy(newAddr[index],addrTmp->values[index]));
    }

    addr->Identifiers[position]->values = newAddr;

    return addr;
}

/*!
 * \fn ListIdentifier setTypeOfIdentifier(ListIdentifier addr, int position, int type)
 * \brief Fonction qui modifie le type de l'identificateur
*/
ListIdentifier setArraySizeOfIdentifier(ListIdentifier addr, int position, int arraySize)
{
    log_trace("setArraySizeOfIdentifier (ListIdentifier %p, int %d,int %d)",addr,position,arraySize)
    CHECKPOINTER(addr);

    if(arraySize < 1){
        log_error("arraySize : %d : %d",arraySize,1)
        perror("setTypeOfIdentifier : arraySize must be more than 1.");
        exit(EXIT_FAILURE);
    }

    if((position == NOTFOUND) && (position >= addr->numberIdentifiers)){
        log_error("Position out of range : position : %d",position)
        perror("setArraySizeOfIdentifier : can not set type of non-existent identifier.");
        exit(EXIT_FAILURE);
    }

    addr->Identifiers[position]->arraySize = arraySize;

    return addr;
}

/*!
 * \fn void printListIdentifier(ListIdentifier addr)
 * \brief Fonction affiche l'état actuelle de la liste des identificateurs
*/
void printListIdentifier(ListIdentifier addr)
{
    log_trace("printListIdentifier (ListIdentifier %p)",addr)
    CHECKPOINTER(addr);

    int index,valuesIndex;
    int size = addr->numberIdentifiers;
    int numberValues;

    for(index = 0; index < size; index++){
        printf("index : %d, name %s, values : %s",index,addr->Identifiers[index]->name,addr->Identifiers[index]->values[0]);
        numberValues = addr->Identifiers[index]->arraySize;

        for(valuesIndex = 1; valuesIndex < numberValues; valuesIndex++){
            printf(", %s",addr->Identifiers[index]->values[valuesIndex]);
        }
        printf("\n");
    }
}
