#ifndef SOS_COMPILATION_H
#define SOS_COMPILATION_H

#include <stdarg.h>

#include "listRangeVar.h"
#include "listInstructionMips.h"
#include "boolExpr.h"
#include "memory.h"
#include "marker.h"

#define CHECK_TYPE(op) do { if (((op) != 1)) {log_error("not good type."); setErrorFailure(); return NULL;} } while (0);

#define CALCULATE_OFFSET(slot) getMipsOffset(slot, *listRangeVariable->cursor->memoryCurrentStackOffset)

void initStruct();

void freeStruct();

/**
 * @brief adds an identifier to the list
 *
 * @param char* : name, identifier's name
 * @param MemorySlot : memorySlot
 * @param bool : local
 * @return
 */
MemorySlot assign(char *name, MemorySlot memorySlot, bool local);

/**
 *
 * @param char* : name, identifier's name
 * @param MemorySlot : offset, address of the offset
 * @param MemorySlot : concat
 * @return
 */
int assignArrayValue(char* name, MemorySlot offset, MemorySlot concat);

/**
 * @brief Executes an echo
 *
 * @param MemorySlotList : list, list to echo
 * @return 0 if success, -1 if it fails
 */
int doEcho(MemorySlotList list);

/**
 * @brief Executes an operation
 *
 * @param memory_space_t : left, left element of the operation
 * @param int : operation, the operation to effectuate
 * @param memory_space_t : right, right element of the operation
 * @return MemorySlot, the resulting memorySlot
 */
MemorySlot doOperation(struct memory_space_t * left, int operation, struct memory_space_t * right);

/**
 *
 * @param const char* : val, value to parse
 * @return int, the size of the parsed table
 */
int doParseTableInt(const char *val);

/**
 * @brief declares a static array
 *
 * @param char* : id, identifier
 * @param int : size, size of the array
 * @return 0 if success, -1 if it fails
 */
int doDeclareStaticArray(char *id, int size);

/**
 * @brief reads an array
 *
 * @param char* : id, identifier of the array
 * @param MemorySlot : offset
 * @return 0 if success, -1 if it fails
 */
int doArrayRead(char *id, MemorySlot offset);

/**
 * @brief get the variable at the specified address
 *
 * @param char* : id, identifier of the variable
 * @param bool : negative, if the var is negative
 * @param bool : isOperandInt,
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
MemorySlot doWriteInt(char *val);

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

int doMarkerLoop(int blockType, Marker mark);

Marker doMarkerArg();

int doMarkerTestFor();

int doForIdAssign(Marker mark);

int doMarkerEndLoop();

int doMarkerDone();

RangeVariable getLastBlockFunction();

/**
 *
 * @param list
 * @return
 */
Marker doMarkerForList(MemorySlotList list);

/**
 *
 * @param mark
 * @return
 */
int doDeleteLocalOffset(Marker mark);

int addBlock(int blockType);

int deleteBlock();

/**
 *
 * @param id Id label
 * @return
 */
Marker doFunctionStartMarker(char* id);

/**
 *
 * @param marker
 * @return
 */
int doDeclareFunction(Marker marker);

/**
 *
 * @param id
 * @param list
 * @return
 */
int doFunctionCall(char* id, MemorySlotList list);

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

/**
 *
 * @param slot
 * @return
 */
MemorySlot doGetArgument(MemorySlot slot, bool negative, bool isOperandInt);

/**
 *
 * @param slot
 * @return
 */
int doReturn(MemorySlot slot);

/**
 *
 * @return
 */
MemorySlot doGetLastStatus();

/**
 *
 * @param id
 * @return
 */
Marker getOrCreateForIdMarker(char* id);

/**
 *
 */
void printSymbolTable();

#endif //SOS_COMPILATION_H
