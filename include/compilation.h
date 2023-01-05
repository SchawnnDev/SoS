#ifndef SOS_COMPILATION_H
#define SOS_COMPILATION_H

#include <stdarg.h>

#include "listRangeVar.h"
#include "listInstructionMips.h"
#include "boolExpr.h"
#include "memory.h"

#define CHECK_TYPE(op) do { if (((op) != 1)) {log_error("not good type."); return NULL; } } while (0);

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
MemorySlot assign(char* name, MemorySlot memorySlot);

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
 * @param val
 * @return
 */
int doParseTableInt(const char *val);

/**
 *
 * @param id
 * @param size
 * @return
 */
int doDeclareStaticArray(char *id, int size);

/**
 *
 * @return
 */
int doArrayRead();

/**
 *
 * @return
 */
MemorySlot doGetVariableAddress(char *id, bool negative);

/**
 *
 * @param val
 * @return
 */
MemorySlot doWriteInt(const char *val);

/**
 *
 * @param slot
 * @param negative
 * @return
 */
MemorySlot doUnaryCheck(MemorySlot slot, bool negative);

/**
 *
 * @param id
 * @return
 */
int doStringRead(const char *id);


/**
 *
 * @return
 */
int getValues();

int setMarker();

int doMarkerThen();
int doMarkerElse();
/**
 *
 * @param addr
 * @param boolExpr
 * @return
 */
MemorySlot doBoolExpression(MemorySlot left, boolExpr_t boolExpr, MemorySlot right);

/**
 *
 * @param slot NULL if no exit code
 * @return
 */
int doExit(MemorySlot slot);

/**
 *
 * @param into Register ($t0-t3 overwritten)
 * @param memorySlot Skip offset in listTmp
 * @return
 */
MemorySlot doConcatenation(MemorySlotList slotList);

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

int parseInt32(const char *word, int *err);

/**
 *
 * @param id
 * @return
 */
MemorySlot getOrCreateMemorySlot(char* id);

/**
 *
 * @param str
 * @return
 */
MemorySlot addStringToMemory(const char *str);

/**
 *
 * @param str
 * @return
 */
MemorySlot addWordToMemory(const char *str);

/**
 *
 * @param slot
 * @return
 */
MemorySlot convertIntToString(MemorySlot slot);

/**
 *
 * @param left
 * @param op
 * @param right
 * @return
 */
MemorySlot doConcatBoolExpr(MemorySlot left, boolExpr_t op, MemorySlot right);

#endif //SOS_COMPILATION_H
