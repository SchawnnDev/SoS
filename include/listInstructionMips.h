//
// Created by the_s on 26/12/2022.
//

#ifndef SOS_LISTINSTRUCTIONMIPS_H
#define SOS_LISTINSTRUCTIONMIPS_H

#include "utils.h"
#include "variable.h"

typedef struct data_t{
    int numberData;
    char* lineData[DATA_TAB_MAX];
    struct data_t* previousData;
} *Data;

typedef struct text_t{
    int numberText;
    char* lineText[TEXT_TAB_MAX];
    struct text_t* previousText;
} *Text;

typedef struct code_t{
    int numberCode;
    char* lineCode[CODE_TAB_MAX];
    int unDefineGoto[CODE_TAB_MAX];
    struct code_t* previousCCode;
} *Code;

typedef struct{
    Data cursorData;
    Text cursorText;
    Code cursorCode;
}listInstruction_t, *ListInstruction;

#endif //SOS_LISTINSTRUCTIONMIPS_H
