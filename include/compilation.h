#ifndef SOS_COMPILATION_H
#define SOS_COMPILATION_H

#include <stdarg.h>

#include "listRangeVar.h"

#define CHECK_TYPE(op) do { if (((op) != 1)) {log_error("not good type."); return EXIT_FAILURE; } } while (0);
// Global variables

ListRangeVariable listRangeVariable;
ListTmp listTmp;
FILE *outFile;

void initStruct();

void AddIdentifier(char* name);

void AddIntoListTmp(char* value);

void SetValuesFromListTmp(char* name);

int compile(FILE *inputFile, FILE *outputFile);

/**
 *
 * @param pattern
 * @param string
 * @return -1 if error, 0 if not matching, 1 if matching
 */
int checkRegex(const char* pattern, const char* string);

// Functions

int checkWordIsId(const char* word);

int checkWordIsInt(const char* word);

#endif //SOS_COMPILATION_H
