#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../Utility/vectors.h"
#include "../Materials/material.h"

/**
 * Triangle data structure:
 */
typedef struct {
    point3 a, b, c;
    material* mat;
} triangle;

triangle* triangle_create(point3, point3, point3, material*);

#endif