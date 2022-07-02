// Copyright (C) 2022  licktheroom //

#ifndef LEVEL_DATA_HEADER
#define LEVEL_DATA_HEADER

#include <stdbool.h>

#include "enums.h"
#include "vector.h"

struct ship_level_data
{
    vec2 pos;
    Ship_type type;
    bool directions[4];
};

struct level_data
{
    int blue_ship_n;
    int red_ship_n;

    struct ship_level_data blue_ships[25];
    struct ship_level_data red_ships[25];
};

static const struct level_data lev_data =
{
    .blue_ship_n = 1,
    .red_ship_n = 0,

    .blue_ships =
    {
        {.pos = {.x = 0, .y = 2}, .type = SHIP_TYPE_SCOUT, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
    },

    .red_ships =
    {
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
        {.pos = {.x = 0, .y = 0}, .type = SHIP_TYPE_NONE, .directions = {false, false, false, false}},
    }
};

#endif
