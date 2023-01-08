#ifndef SOS_MARKER_H
#define SOS_MARKER_H

#include "listInstructionMips.h"

typedef struct marker_t *Marker;

struct marker_t {
    Code code;
    int index;

    char* lbl;
};

/**
 * @brief Creates and allocate a new marker.
 *
 * @return Marker, the created marker
 */
Marker newMarker();

/**
 * @brief Destroys and frees the marker
 * @param Marker : marker, marker to destroy
 */
void destroyMarker(Marker marker);

#endif //SOS_MARKER_H
