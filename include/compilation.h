#ifndef SOS_COMPILATION_H
#define SOS_COMPILATION_H

#include "listRangeVar.h"

ListRangeVariable listRangeVariable;
ListTmp listTmp;

int initStruct();

void AddIdentifier(char* name);

void AddIntoListTmp(char* value);

void SetValuesFromListTmp(char* name);

#endif //SOS_COMPILATION_H
