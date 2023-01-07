#include "marker.h"

Marker newMarker()
{
    Marker marker;
    CHECKPOINTER(marker = malloc(sizeof(struct marker_t)))
    marker->lbl = NULL;
    return marker;
}

void destroyMarker(Marker marker)
{
    if(marker == NULL) return;
    if(marker->lbl != NULL) free(marker->lbl);
    free(marker);
}