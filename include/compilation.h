#ifndef SOS_COMPILATION_H
#define SOS_COMPILATION_H

#include <stdarg.h>

#include "listRangeVar.h"
#include "listInstructionMips.h"
#include "boolExpr.h"
#include "memory.h"

#define CHECK_TYPE(op) do { if (((op) != 1)) {log_error("not good type."); return NULL; } } while (0);

void initStruct();

/**
 *
 * @param name
 * @return
 */
MemorySlot assign(char *name, MemorySlot memorySlot, bool local);

/**
 *
 * @param name
 * @param offset
 * @param concat
 * @return
 */
int assignArrayValue(char* name, MemorySlot offset, MemorySlot concat);

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
 * @param id
 * @param offset
 * @return
 */
int doArrayRead(char *id, MemorySlot offset);

/**
 *
 * @param id
 * @param negative
 * @param isOperandInt
 * @return
 */
MemorySlot doGetVariableAddress(char *id, bool negative, bool isOperandInt);

/**
 *
 * @param id
 * @param offset
 * @return
 */
MemorySlot doGetArrayAddress(char *id, MemorySlot offset, bool negative,
                             bool isOperandInt);

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

int doMarkerTest();

int doMarkerElse();

int doMarkerFi();

int doMarkerEndInstruction();

int doMarkerLoop();

int doMarkerEndLoop();

int doMarkerDone();

/**
 *
 * @param left
 * @param boolExpr
 * @param right
 * @return
 */
MemorySlot doBoolExpression(MemorySlot left, boolExpr_t boolExpr, MemorySlot right);

/**
 *
 * @param left
 * @param boolExpr
 * @param right
 * @return
 */
MemorySlot doEmptyBoolExpression( boolExpr_t boolExpr, MemorySlot right);

int doNegBoolExpression();
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
Identifier getIdentifier(char *id, bool create, bool local);

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

#endif //SOS_COMPILATION_H
