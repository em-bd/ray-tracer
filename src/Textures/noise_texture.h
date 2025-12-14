#ifndef NOISE_TEXTURE_H
#define NOISE_TEXTURE_H

#include "../Utility/perlin.h"
#include "texture.h"

typedef struct {
    perlin* perlin;
    double scale;
} noise_texture;

texture* noise_create(double);

#endif