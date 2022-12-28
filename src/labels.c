#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include "labels.h"
#include "utils.h"

int labelCount = 0;
int size = LABEL_NAME_LEN + 1;
int currDiz = 10;

const char* createNewLabel()
{
    if(labelCount >= currDiz)
    {
        currDiz *= 10;
        size++;
    }

    char* result;
    CHECKPOINTER(result = malloc(size))
    CHECK(snprintf(result, size, LABEL_NAME, labelCount))
    labelCount++;
    return result;
}

