#ifndef __LISTIDEN_H
#define __LISTIDEN_H

#include "utils.h"
#include "variable.h"

typedef struct {
    char * nom;
    char * valeur;
} identificateur_t, *Identificateur;

typedef struct {
    Identificateur lesIdentificateurs[IDEN_MAX];
    int nombreIden; 
} listIdentificateur_t, *ListIdentifacteur;

#endif