#ifndef METAL_H
#define METAL_H

#include <stdio.h>
#include <stdlib.h>
#include "../Utility/color.h"

/**
 * Metal material data structure:
 */
typedef struct {
    color albedo;
    double fuzz;
} metal;

metal* metal_create(color, double);

#endif