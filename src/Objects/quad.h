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

bool is_interior(double, double, hit_record*);

#endif