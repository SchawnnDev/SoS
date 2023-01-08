#ifndef SOS_LOADREGISTERSFROMSTACK_ASM_H
#define SOS_LOADREGISTERSFROMSTACK_ASM_H
#include "asm.h"

/**
 * Loads all the registers previously saved
 * on the stack, into the registers again.
 *
 * @return 0 if success, -1 if it fails
 */
int asm_writeLoadRegistersFromStackFunction();

#endif //SOS_LOADREGISTERSFROMSTACK_ASM_H
