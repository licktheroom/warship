#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include <stdbool.h>

typedef struct
{
    int x;
    int y;
} vec2;

unsigned int VEC_dist(vec2 a, vec2 b);
bool VEC_eq(vec2 a, vec2 b);

#endif
