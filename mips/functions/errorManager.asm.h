#ifndef SOS_ERRORMANAGER_ASM_H
#define SOS_ERRORMANAGER_ASM_H
#include "functions.asm.h"

/**
 * Occurs when trying to access data outside
 * the scope of declared data.
 *
 * @return 0 if success, -1 if it fails
 */
int asm_writeOutOfBoundsErrorFunction();

/**
 * Occurs when the atoi function try to
 * transform a string that does not represent
 * a number.
 *
 * @return 0 if success, -1 if it fails
 */
int asm_writeNotANumberErrorFunction();

/**
 * Occurs when an array element is accessed at
 * an index outside of the declared scope.
 *
 * @return 0 if success, -1 if it fails
 */
int asm_writeArrayElementNotAllocatedErrorFunction();

/**
 * Occurs when an argument is accessed inside a
 * function which is not provided by it's call.
 *
 * @return 0 if success, -1 if it fails
 */
int asm_writeNonExistentArgumentErrorFunction();

/**
 *  Error printer: it prints the error message
 *  and exits the program.
 *
 * @return 0 if success, -1 if it fails
 */
int asm_writePrintErrorAndExit();

#endif //SOS_ERRORMANAGER_ASM_H
