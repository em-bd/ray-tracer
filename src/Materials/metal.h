#ifndef METAL_H
#define METAL_H

#include <stdio.h>
#include <stdlib.h>
#include "material.h"

/**
 * Metal material data structure:
 */
typedef struct {
    color albedo;
    double fuzz;
} metal;

material* metal_create(color, double);

#endif