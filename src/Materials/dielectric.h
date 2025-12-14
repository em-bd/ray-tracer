#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include <stdio.h>
#include <stdlib.h>

/**
 * Dielectric material data structure:
 */
typedef struct {
    double refraction_index;
} dielectric;

dielectric* dielectric_create(double);

#endif