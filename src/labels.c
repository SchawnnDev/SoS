#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include "labels.h"
#include "utils.h"

int labelCount = 0;
int size = LABEL_NAME_LEN + 1;
int currDiz = 10;

int labelForCount = 0;
int sizeFor = LABEL_NAME_LEN + 1;
int currDizFor = 10;

const char* createNewLabel()
{
    if(labelCount >= currDiz)
    {
        currDiz *= 10;
        size++;
    }

    char* result;
    CHECKPOINTER(result = malloc(size))
    CHECK_ERROR_RETURN(NULL)
    CHECK(snprintf(result, size, LABEL_NAME, labelCount))
    CHECK_ERROR_RETURN(NULL)
    labelCount++;
    return result;
}

const char* createNewForLabel()
{
    if(labelForCount >= currDizFor)
    {
        currDizFor *= 10;
        sizeFor++;
    }

    char* result;
    CHECKPOINTER(result = malloc(sizeFor))
    CHECK_ERROR_RETURN(NULL)
    CHECK(snprintf(result, sizeFor, LABEL_FOR, labelForCount))
    CHECK_ERROR_RETURN(NULL)
    labelForCount++;
    return result;
}

