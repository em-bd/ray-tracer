#ifndef EMISSIVE_H
#define EMISSIVE_H

#include "material.h"
#include "../Textures/solid_color.h"

typedef struct {
    texture* tex;
} emissive;

material* emissive_create(texture*);

material* emissive_create_color(color);

color emitted(emissive*, double*, double*, point3*);

#endif