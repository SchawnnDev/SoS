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
 * \fn int searchIdentifierPosition(ListIdentifier addr, char* name)
 * \brief Fonction qui recherche la position de l'Identifier dans la liste des identificateurs
*/
int searchIdentifierPosition(ListIdentifier addr, char* name)
{
    log_trace("searchIdentifierPosition (ListIdentifier %p, char* %s)",addr,name)

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

    if(searchIdentifierPosition(addr, name) != NOTFOUND){
        log_error("Identifier found : name : %s, position : %d",name,index)
        perror("addIntoListIdentifier : can not add existing identifier.");
        exit(EXIT_FAILURE);
    }

    addr->Identifiers[addr->numberIdentifiers] = initIdentifier(name);
    addr->numberIdentifiers++;

    return addr;
}

/*!
 * \fn ListIdentifier setTypeOfIdentifier(ListIdentifier addr, char* name,int type)
 * \brief Fonction qui modifie le type de l'identificateur
*/
ListIdentifier setTypeOfIdentifier(ListIdentifier addr, char* name,int type)
{
    log_trace("setTypeOfIdentifier (ListIdentifier %p, char* %s,int %d)",addr,name,type)
    CHECKPOINTER(addr);
    CHECKPOINTER(name);

    if((type < UNSET ) || (type > MAXTYPEVALUE)){
        log_error("type : %d : %d > type < %d",type,UNSET,MAXTYPEVALUE)
        perror("setTypeOfIdentifier : Value du type n'est pas admise par le compilateur.");
        exit(EXIT_FAILURE);
    }


    int position = searchIdentifierPosition(addr, name);

    if(position == NOTFOUND){
        log_error("Identifier not found : name : %s, position : %d",name,index)
        perror("setTypeOfIdentifier : can not set type of non-existent identifier.");
        exit(EXIT_FAILURE);
    }

    addr->Identifiers[position]->type = type;

    return addr;
}

/*!
 * \fn ListIdentifier setValuesOfIdentifierFromListTmp(ListIdentifier addr, char* name,ListTmp addrTmp)
 * \brief Fonction remplie le tableau des valeurs de l'identificateur grave à la liste temporaire
*/
ListIdentifier setValuesOfIdentifierFromListTmp(ListIdentifier addr, char* name,ListTmp addrTmp)
{
    log_trace("setValuesOfIdentifierFromListTmp (ListIdentifier %p, char* %s, ListTmp %p)",addr,name,addrTmp)
    CHECKPOINTER(addr);
    CHECKPOINTER(name);
    CHECKPOINTER(addrTmp);

    int position = searchIdentifierPosition(addr, name);

    if(position == NOTFOUND){
        log_error("Identifier not found : name : %s, position : %d",name,index)
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
 * \fn ListIdentifier setTypeOfIdentifier(ListIdentifier addr, char* name,int type)
 * \brief Fonction qui modifie le type de l'identificateur
*/
ListIdentifier setArraySizeOfIdentifier(ListIdentifier addr, char* name,int arraySize)
{
    log_trace("setArraySizeOfIdentifier (ListIdentifier %p, char* %s,int %d)",addr,name,arraySize)
    CHECKPOINTER(addr);
    CHECKPOINTER(name);

    if(arraySize < 1){
        log_error("arraySize : %d : %d",arraySize,1)
        perror("setTypeOfIdentifier : arraySize must be more than 1.");
        exit(EXIT_FAILURE);
    }

    int position = searchIdentifierPosition(addr, name);

    if(position == NOTFOUND){
        log_error("Identifier not found : name : %s, position : %d",name,index)
        perror("setTypeOfIdentifier : can not set type of non-existent identifier.");
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
