#ifndef __UTILS_H
#define __UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include <log.h>

/*!
 * \fn CHECK
 * \brief Permet de tester une valeur critique du programme
 */
#define CHECK(op) do { if (((op) == -1)) raler (1, #op); } while (0);

/*!
 * \fn CHECKPOINTER
 * \brief Permet de tester un pointeur critique du programme
 */
#define CHECKPOINTER(op) do { if (((op) == NULL)) raler (1, #op); } while (0);

void raler(int syserr, const char *msg, ...);

/**
 *
 * @param str
 * @return
 */
int countWithoutBackslash(char *str);

#endif