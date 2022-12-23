#include "../include/listTmp.h"

/*!
 * \fn ListTmp initListTmp()
 * \brief Fonction qui initialise la structure des valeurs temporaires
*/
ListTmp initListTmp()
{
    log_trace("initListTmp (void)")

    ListTmp addr;
    CHECKPOINTER(addr = (ListTmp)malloc(sizeof(listTmp_t)));
    addr->numberValues = 0;
    return addr;
}

/*!
 * \fn void cleanListTmp(ListTmp addr)
 * \brief Fonction qui libère la mémoire de la structure des valeurs temporaire
*/
void cleanListTmp(ListTmp addr)
{
    log_trace("cleanListTmp (ListTmp %p)",addr)
    CHECKPOINTER(addr);

    int index;
    int size = addr->numberValues;
    for(index = 0; index < size; index++){
        free(addr->values[index]);
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
    CHECKPOINTER(value);

    if(addr->numberValues == TMP_TAB_MAX){
        log_error("numberValues : %d, %d",addr->numberValues,TMP_TAB_MAX)
        perror("addIntoListTmp : to many values.");
        return RETURN_FAILURE;
    }

    ulong size = strlen( value ) + 1;
    CHECKPOINTER(addr->values[addr->numberValues] = (char*)malloc(sizeof(char) * size));
    CHECKPOINTER(strcpy(addr->values[addr->numberValues],value));

    addr->numberValues++;

    return RETURN_SUCCESS;
}