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