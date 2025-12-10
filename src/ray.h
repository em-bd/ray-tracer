#ifndef RAY_H
#define RAY_H

#include "vectors.h"

typedef struct {
    point3 orig;
    vec3 dir;
} ray;

point3 at(ray, double);

#endif