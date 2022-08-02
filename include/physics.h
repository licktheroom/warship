// Copyright (C) 2022  licktheroom //

#ifndef PHY_HEADER
#define PHY_HEADER

#include <stdbool.h>

#include "structs.h"

void phy_update_ship_children(Ship * sh);

void phy_move_ship(Ship * sh);

int phy_get_direction(bool current[4], bool last_check[4]);

#endif
