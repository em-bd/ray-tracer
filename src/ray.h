#ifndef RAY_H
#define RAY_H

#include "vectors.h"

/**
 * Ray data structure:
 */
typedef struct {
    point3 orig;
    vec3 dir;
} ray;

point3 at(ray, double);

ray ray_create(vec3, vec3);

#endif