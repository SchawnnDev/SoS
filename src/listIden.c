#include "../include/listIden.h"

/*!
 * \fn ListIdentifier initListIdentifier()
 * \brief Fonction qui initialise la structure des Identificateurs
*/
ListIdentifier initListIdentifier()
{
    ListIdentifier addr;
    CHECKPOINTER(addr = (ListIdentifier)malloc(sizeof(listIdentifier_t)));
    addr->numbreIden = 0;
    return addr;
}

/*!
 * \fn void cleanListIdentifier(ListIdentifier addr)
 * \brief Fonction qui libère la mémoire de la structure des Identificateurs
*/
void cleanListIdentifier(ListIdentifier addr)
{
    free(addr);
}

/*!
 * \fn ListIdentifier addIntoListIdentifier(ListIdentifier addr, char* name, char* value)
 * \brief Fonction qui crée un Identifier et l'ajoute en fin de la liste des identificateurs
*/
ListIdentifier addIntoListIdentifier(ListIdentifier addr, char* name, char* value, int type)
{
    CHECKPOINTER(addr);

    if((type < UNSET ) || (type > MAXTYPEVALUE)){
        perror("addIntoListIdentifier : Value du type n'est pas admise par le compilateur.");
        exit(EXIT_FAILURE);
    }

    Identifier identifierAddr;
    CHECKPOINTER(identifierAddr = (Identifier)malloc(sizeof(identifier_t)));
    int size = strlen( name ) + 1;

    /* To Do*/
}