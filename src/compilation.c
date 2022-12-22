#include "compilation.h"

int initStruct()
{
    log_trace("Started initStruct")
    listRangeVariable = initListRangeVariable();
    listTmp = initListTmp();
    return 0;
}

void AddIdentifier(char* name)
{
    addIdentifier(listRangeVariable, name);
}

void AddIntoListTmp(char* value)
{
    addIntoListTmp(listTmp, value);
}

void SetValuesFromListTmp(char* name)
{
    addIdentifier(listRangeVariable, name);
    setValuesFromListTmp(listRangeVariable,name,listTmp);
    cleanListTmp(listTmp);
    listTmp = initListTmp();
}