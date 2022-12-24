#include "../include/listTmp.h"

/*!
 * \fn TmpValues initTmpValues()
 * \brief Fonction qui initialise la structure des valeurs temporaires
*/
TmpValues initTmpValues(TmpValues previousTmpValues)
{
    log_trace("initTmpValues (TmpValues %p)",previousTmpValues)

    TmpValues addr;
    CHECKPOINTER(addr = (TmpValues)malloc(sizeof( struct tmpValues_t)));
    addr->previousTmpValues = previousTmpValues;
    addr->numberValues = 0;
    return addr;
}

/*!
 * \fn void cleanTmpValues(TmpValues addr)
 * \brief Fonction qui libère la mémoire de la structure des valeurs temporaire
*/
void cleanTmpValues(TmpValues addr)
{
    log_trace("cleanTmpValues (TmpValues %p)",addr)
    CHECKPOINTER(addr);

    int index;
    int size = addr->numberValues;
    for(index = 0; index < size; index++){
        free(addr->values[index]);
    }

    free(addr);
}

/*!
 * \fn ListTmp initListTmp()
 * \brief Fonction qui initialise la liste des valeurs temporaire
*/
ListTmp initListTmp()
{
    log_trace("initListRangeVariable (void)")

    ListTmp addr;
    CHECKPOINTER(addr = (ListTmp)malloc(sizeof(listTmp_t)));
    addr->cursor = initTmpValues(NULL);

    return addr;
}

/*!
 * \fn void cleanListTmp(ListTmp addr)
 * \brief Fonction qui libère la mémoire de la liste des valeurs temporaires
*/
void cleanListTmp(ListTmp addr)
{
    log_trace("cleanListTmp (ListTmp %p)", addr)
    CHECKPOINTER(addr);

    TmpValues tmp, addrToFree = addr->cursor;
    while(addrToFree != NULL){
        tmp = addrToFree->previousTmpValues;
        cleanTmpValues(addrToFree);
        addrToFree = tmp;
    }

    free(addr);
}

/*!
 * \fn int addIntoListTmp(ListTmp addr, char* value)
 * \brief Fonction qui permet d'ajoute en fin de la liste des valeurs temporaires
*/
int addIntoListTmp(ListTmp addr, char* value)
{
    log_trace("addIntoListTmp (ListTmp %p, char* %s)",addr,value)
    CHECKPOINTER(addr);
    CHECKPOINTER(addr->cursor);
    CHECKPOINTER(value);

    if(addr->cursor->numberValues == TMP_TAB_MAX){
        log_error("numberValues : %d, %d",addr->cursor->numberValues,TMP_TAB_MAX)
        perror("addIntoListTmp : to many values.");
        return RETURN_FAILURE;
    }

    ulong size = strlen( value ) + 1;
    CHECKPOINTER(addr->cursor->values[addr->cursor->numberValues] = (char*)malloc(sizeof(char) * size));
    CHECKPOINTER(strcpy(addr->cursor->values[addr->cursor->numberValues],value));

    addr->cursor->numberValues++;

    return RETURN_SUCCESS;
}

/*!
 * \fn void addIntoListTmp(ListTmp addr, char* value)
 * \brief Fonction qui permet d'ajoute une structure de valeur temporaire
*/
void addListTmp(ListTmp addr)
{
    log_trace("addListTmp (ListTmp %p)",addr)
    CHECKPOINTER(addr);

    addr->cursor = initTmpValues(addr->cursor);
}

/*!
 * \fn int deleteListTmp(ListTmp addr)
 * \brief Fonction qui permet supprimer la dernière structure de valeur temporaire
 *
 * \param addr : ListTmp, la liste des valeurs temporaires
*/
int deleteListTmp(ListTmp addr)
{
    log_trace("deleteListTmp (ListTmp %p)", addr)
    CHECKPOINTER(addr);

    if(addr->cursor == NULL){
        log_error("cursor : %p",addr->cursor)
        perror("deleteListTmp : there is no TmpValues to delete.");
        return RETURN_FAILURE;
    }

    TmpValues tmp = addr->cursor;
    addr->cursor = tmp->previousTmpValues;
    cleanTmpValues(tmp);

    return RETURN_SUCCESS;
}