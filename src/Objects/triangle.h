#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object.h"

/**
 * Triangle data structure:
 */
typedef struct {
    point3 a, b, c;
    material* mat;
    aabb bbox;
} triangle;

object* triangle_create(point3, point3, point3, material*);

#endif