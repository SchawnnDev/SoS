#ifndef SOS_COMPILATION_H
#define SOS_COMPILATION_H

#include <stdarg.h>

#include "listRangeVar.h"
#include "listInstructionMips.h"
#include "boolExpr.h"

#define CHECK_TYPE(op) do { if (((op) != 1)) {log_error("not good type."); return EXIT_FAILURE; } } while (0);


void initStruct();

void addIdOrder(char *name);

void setTypeOrder(int type);

/*!
 * \fn void setCurrentOperation(int operation)
 * @param operation Current operation
 */
void setCurrentOperation(int operation);

/**
 *
 * @param expr
 */
void setCurrentBooleanExpression(boolExpr_t expr);

/*!
 * \fn void addTmpValuesListTmp
 * \brief Fonction qui ajoute une structure de valeur temporaire
*/
void addTmpValuesListTmp();

void assign();

void addValueIntoListTmp(char *value);

void echo();

void doOperation();

/**
 *
 * @return
 */
int getValues();

/**
 *
 * @param addr
 * @param boolExpr
 * @return
 */
int doBoolExpression();

// Utils

int compile(FILE *inputFile, FILE *outputFile);

/**
 *
 * @param pattern
 * @param string
 * @return -1 if error, 0 if not matching, 1 if matching
 */
int checkRegex(const char *pattern, const char *string);

// Functions

int checkWordIsId(const char *word);

int checkWordIsInt(const char *word);

int parseInt32(const char *word);

/**
 *
 * @param str
 * @return
 */
int writeQuotedString(const char* str);

/**
 *
 * @param str
 * @return
 */
int writeApostrophedString(const char* str);

#endif //SOS_COMPILATION_H
