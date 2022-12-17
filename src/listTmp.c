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