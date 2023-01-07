#ifndef SOS_MARKER_H
#define SOS_MARKER_H

#include "listInstructionMips.h"

typedef struct marker_t *Marker;

struct marker_t {
    Code code;
    int index;

    char* lbl;
};

Marker newMarker();

void destroyMarker(Marker marker);

#endif //SOS_MARKER_H
