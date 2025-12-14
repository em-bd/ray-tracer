#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "material.h"

/**
 * Dielectric material data structure:
 */
typedef struct {
    double refraction_index;
} dielectric;

material* dielectric_create(double);

#endif