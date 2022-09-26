// Copyright (C) 2022  licktheroom //

#include "include/engine.h"

#include <math.h>
#include <stdbool.h>

int abs(int a)
{
    if(a < 0)
        return -a;

    return a;
}

uint v2_distance(vec2 a, vec2 b)
{
    int x = pow(abs(a.x-b.x), 2);
	int y = pow(abs(a.y-b.y), 2);

	return sqrt(x+y);
}
