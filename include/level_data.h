// Copyright (C) 2022  licktheroom //

#ifndef LEVEL_DATA_HEADER
#define LEVEL_DATA_HEADER

#include <stdbool.h>
#include "engine.h"

struct t_ship_level_data
{
    vec2 pos;
    Ship_type type;
    bool directions[4];
};

struct t_level_data
{
    int blue_ship_n;
    int red_ship_n;

    struct t_ship_level_data blue_ships[25];
    struct t_ship_level_data red_ships[25];
};

static const struct t_level_data lev_data =
{
    .blue_ship_n = 3,
    .red_ship_n = 1,

    .blue_ships =
    {
        {.pos = {.x = 10, .y = 2}, .type = SHIP_TYPE_SCOUT, .directions = {false, false, false, false}},
        {.pos = {.x = 8, .y = 15}, .type = SHIP_TYPE_WARSHIP, .directions = {false, false, false, false}},
        {.pos = {.x = 15, .y = 23}, .type = SHIP_TYPE_CARRIER, .directions = {false, false, false, false}},
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
        {.pos = {.x = 200, .y = 10}, .type = SHIP_TYPE_SCOUT, .directions = {false, false, true, false}},
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
