#ifndef STRUCTURE_HEADER
#define STRUCTURE_HEADER

#include <stdbool.h>

#include "vector.h"
#include "enums.h"

typedef struct
{
    vec2 position;
    vec2 children_positions[52];

    int children_size;

    bool directions[4];
    bool moving;
    bool controlled;

    Ship_type type;
} Ship;

#endif
