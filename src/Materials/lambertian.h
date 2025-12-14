#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include <stdio.h>
#include <stdlib.h>
#include "../Textures/texture.h"

/**
 * Lambertian data structure:
 */
typedef struct {
    texture* tex;
} lambertian;

lambertian* lambertian_create(texture*);

#endif