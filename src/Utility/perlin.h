#ifndef PERLIN_H
#define PERLIN_H

#include "vectors.h"
#include "utils.h"

typedef struct {
    int point_count;
    double rand_float[256];
    int* perm_x;
    int* perm_y;
    int* perm_z;
} perlin;

perlin* perlin_create();

double noise(perlin*, point3*);

#endif