#include "include/vector.h"

#include <math.h>
#include <stdbool.h>

int abs(int a)
{
    if(a < 0)
        return -a;

    return a;
}

unsigned int VEC_dist(vec2 a, vec2 b)
{
    int x = pow(abs(a.x-b.x), 2);
	int y = pow(abs(a.y-b.y), 2);

	return sqrt(x+y);
}

bool VEC_eq(vec2 a, vec2 b)
{
    if(a.x == b.x && a.y == b.y)
        return true;

    return false;
}
