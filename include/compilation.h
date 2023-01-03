#ifndef SOS_COMPILATION_H
#define SOS_COMPILATION_H

#include <stdarg.h>

#include "listRangeVar.h"
#include "listInstructionMips.h"
#include "boolExpr.h"
#include "memory.h"

#define CHECK_TYPE(op) do { if (((op) != 1)) {log_error("not good type."); return EXIT_FAILURE; } } while (0);

void initStruct();

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

/**
 *
 * @param name
 * @return
 */
MemorySlot assign(char* name, MemorySlotList list);

/**
 *
 * @return
 */
int doEcho(MemorySlotList list);

/**
 *
 */
MemorySlot doOperation(struct memory_space_t * left, int operation, struct memory_space_t * right);

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
MemorySlot doGetVariableAddress(char* id);

/**
 *
 * @param val
 * @return
 */
MemorySlot doWriteInt(const char *val);

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
 * @param memorySlot Skip offset in listTmp
 * @return
 */
MemorySlot doConcatenation(MemorySlot memorySlot, MemorySlotList slotList);

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
MemorySlot addStringToMemory(const char *str);

#endif //SOS_COMPILATION_H
