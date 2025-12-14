#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../Utility/vectors.h"

/**
 * Triangle data structure:
 */
typedef struct {
    vec3 a, b, c;
    vec3 n;
} triangle;

#endif