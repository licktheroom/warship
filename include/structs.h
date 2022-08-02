// Copyright (C) 2022  licktheroom //

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
    int pointing;

    bool last_checked_directions[4];
    bool current_directions[4];
    bool moving;
    bool controlled;

    Ship_type type;
} Ship;

#endif
