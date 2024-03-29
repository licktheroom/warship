// Copyright (C) 2022  licktheroom //

// TODO: Update comments.                                   //
// TODO: Think of a better naming scheme for the functions. //

#include <stdbool.h>

#include "include/ship_offsets.h"

// ship physics

void ship_update_childern(Ship * sh)
{
    // All of these are the same, so to save time I'll only add comments to one
    if(sh->type == SHIP_TYPE_SCOUT)
    {
	// set child set
        sh->children_size = 8;

        for(uint i = 0; i < 8; i++)
        {
            // this will have to be updated to account for rotation
            // names here are key, sh.position is the original position while scout_offsets are the offsets from that original position
            sh->children_positions[i].x = sh->position.x + scout_offsets[sh->pointing][i].x;
            sh->children_positions[i].y = sh->position.y + scout_offsets[sh->pointing][i].y;
        }
    } else if(sh->type == SHIP_TYPE_CARRIER)
    {
        sh->children_size = 52;

        for(uint i = 0; i < 52; i++)
        {
            sh->children_positions[i].x = sh->position.x + carrier_offsets[sh->pointing][i].x;
            sh->children_positions[i].y = sh->position.y + carrier_offsets[sh->pointing][i].y;
        }
    } else if(sh->type == SHIP_TYPE_WARSHIP)
    {
        sh->children_size = 50;

        for(uint i = 0; i < 50; i++)
        {
            sh->children_positions[i].x = sh->position.x + warship_offsets[sh->pointing][i].x;
            sh->children_positions[i].y = sh->position.y + warship_offsets[sh->pointing][i].y;
        }
    }
}

void ship_physics(Ship * sh)
{
    if(sh->current_directions[0])
    {
        sh->position.y--;

        for(uint i = 0; i < sh->children_size; i++)
            sh->children_positions[i].y--;

    } else if(sh->current_directions[1])
    {
        sh->position.y++;

        for(uint i = 0; i < sh->children_size; i++)
            sh->children_positions[i].y++;
    }

    if(sh->current_directions[2])
    {
        sh->position.x--;

        for(uint i = 0; i < sh->children_size; i++)
            sh->children_positions[i].x--;

    } else if(sh->current_directions[3])
    {
        sh->position.x++;

        for(uint i = 0; i < sh->children_size; i++)
            sh->children_positions[i].x++;
    }
}

int ship_current_direction(bool current[4], bool last_checked[4])
{
    // check if this is even needed
    if(current[0] == last_checked[0] && current[1] == last_checked[1] && current[2] == last_checked[2] && current[3] == last_checked[3])
        return -1;

    // check is needed, so set it
    // if no keys are pressed, the direction should remain the last known key
    // directions cheat sheet:
    // up = 0
    // down = 1
    // left = 2
    // right = 3

    if(current[0])
    {
        if(current[2])
            return 5;
        else if(current[3])
            return 7;
        else
            return 6;
    } else if(current[1])
    {
        if(current[2])
            return 3;
        else if(current[3])
            return 1;
        else
            return 2;
    } else if(current[2])
        return 4;
    else if(current[3])
        return 0;

    return -1;
}

// Projectiles //

Proj proj_create(Ship * s, bool team)
{
    Proj a;

    a.position = s->position;
    a.dir = s->pointing;
    a.team = team;
    a.dist_traveled = 0;

    a.parent = s;

    return a;
}
