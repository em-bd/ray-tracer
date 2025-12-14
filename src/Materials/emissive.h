#ifndef EMISSIVE_H
#define EMISSIVE_H

#include "../Textures/texture.h"

typedef struct {
    texture* tex;
} emissive;

emissive* emissive_create(texture*);

emissive* emissive_create_color(color);

color emitted(emissive*, double*, double*, point3*);

#endif