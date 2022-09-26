// Copyright (C) 2022  licktheroom //

/*
 * Overall header managment.
 */

#ifndef ENGINE_HEADERS
#define ENGINE_HEADERS

#include <stdio.h>
#include <stdbool.h>

// Unsigned int //
typedef unsigned int uint;

typedef unsigned char cint;

// Ship identifiers //
typedef enum t_ship_type
{
    SHIP_TYPE_NONE = 1,
    SHIP_TYPE_SCOUT = 2,
    SHIP_TYPE_WARSHIP = 4,
    SHIP_TYPE_CARRIER = 8
} Ship_type;

// Normal vectors //
typedef struct t_vec2
{
    int x;
    int y;
} vec2;

// Needed ship data //
typedef struct t_ship
{
    vec2 position;
    vec2 children_positions[52];

    uint children_size;
    cint pointing;
    cint hp;

    bool last_checked_directions[4];
    bool current_directions[4];
    bool moving;
    bool controlled;

    Ship_type type;
} Ship;

// Needed projectile data //
typedef struct t_proj
{
    vec2 position;

    cint dir;
    bool team;
    uint dist_traveled;

    Ship * parent;
} Proj;

// FUNCTIONS //

extern FILE * get_log_file(uint n); // Gets the output file. //

extern void ship_update_childern(Ship * s); // Update ship childern when loading or turning. //
extern void ship_physics(Ship * s); // Move and see if there is a collision for one ship. //
extern int ship_current_direction(bool current[4], bool last_check[4]); // Where the ship should be pointing. //

extern Proj proj_create(Ship * s, bool team); // Creates a projectile. //

extern uint v2_distance(vec2 a, vec2 b); // Distance between two points on a 2D plane. //

extern bool settings_generate(); // Generates a list of settings. //
extern void settings_clear(); // Clears the settings cache and flushes it to the file. //
extern char * settings_get(const char * name); // Gets a setting's value. //
extern bool settings_set(const char * name, const char * set); // Sets a value. Does not update the file. //

#endif
