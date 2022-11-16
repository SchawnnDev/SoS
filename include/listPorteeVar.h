#ifndef __LISTPORTEEVAR_H
#define __LISTPORTEEVAR_H

#include "utils.h"
#include "variable.h"
#include "listIden.h"

typedef struct {
    ListIdentifacteur laListIdentificateur;
    int niveauPortee;
} porteeVariable_t, *PorteeVariable;

typedef struct {
    PorteeVariable niveauSuivant;
    PorteeVariable niveauPrecedant;
} listPorteeVariable_t, *ListPorteeVariable;

#endif