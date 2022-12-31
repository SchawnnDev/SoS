#ifndef SOS_COMPILATION_H
#define SOS_COMPILATION_H

#include <stdarg.h>

#include "listRangeVar.h"
#include "listInstructionMips.h"
#include "boolExpr.h"
#include "memory.h"

#define CHECK_TYPE(op) do { if (((op) != 1)) {log_error("not good type."); return EXIT_FAILURE; } } while (0);

#define CHECK_VALUE_NOT_ZERO(function_name) \
    CHECKPOINTER(listTmp)                   \
    CHECKPOINTER(listTmp->cursor)                   \
    if (listTmp->cursor->numberValues == 0) \
    { \
        log_error("%s: numberValues : %d", function_name, listTmp->cursor->numberValues) \
        return RETURN_FAILURE; \
    } \

#define CHECK_IDENTIFIER_NOT_ZERO(function_name) \
    CHECKPOINTER(listIdentifierOrder)                   \
    CHECKPOINTER(listIdentifierOrder->cursor)                   \
    if (listIdentifierOrder->cursor->index  == 0) \
    { \
        log_error("%s: you must have one identifier.", function_name) \
        return RETURN_FAILURE; \
    } \

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

int assign();

void addValueIntoListTmp(char *value);

void echo();

/**
 *
 * @return
 */
int doEcho();

/**
 *
 */
MemorySpace doOperation(struct memory_space_t * left, int operation, struct memory_space_t * right);

/**
 *
 * @return
 */
int doOperationAddInt();

/**
 *
 * @return
 */
int doDeclareStaticArray();

/**
 *
 * @return
 */
int doArrayRead();

/**
 *
 * @return
 */
int doGetVariableAddress();

/**
 *
 * @param val
 * @return
 */
MemorySpace doWriteInt(const char *val);

/**
 *
 * @return
 */
int getValues();

int setMarker();

/**
 *
 * @param addr
 * @param boolExpr
 * @return
 */
int doBoolExpression();

/**
 *
 * @param into Register ($t0-t3 overwritten)
 * @param skipOffset Skip offset in listTmp
 * @return
 */
int doConcatenation(const char* into, int skipOffset);

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
int addStringToListTmp(const char *str);

#endif //SOS_COMPILATION_H
