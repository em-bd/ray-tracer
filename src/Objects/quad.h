#ifndef QUAD_H
#define QUAD_H

#include "object.h"

typedef struct {
    point3 Q;
    vec3 u, v, w;
    material* mat;
    vec3 normal;
    double D;
    aabb bbox;
} quad;

object* quad_create(point3, vec3, vec3, material*);

object** box(point3, point3, material*);

#endif