#ifndef PERLIN_H
#define PERLIN_H

#include "vectors.h"
#include "utils.h"

typedef struct {
    int point_count;
    int* perm_x;
    int* perm_y;
    int* perm_z;
    vec3* randvec;
} perlin;

perlin* perlin_create();

double noise(perlin*, point3*);

double turb(perlin*, point3*, int);

#endif