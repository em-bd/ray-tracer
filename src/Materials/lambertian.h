#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "material.h"

/**
 * Lambertian data structure:
 */
typedef struct {
    texture* tex;
} lambertian;

material* lambertian_create(texture*);

#endif