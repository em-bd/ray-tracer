#ifndef NOISE_TEXTURE_H
#define NOISE_TEXTURE_H

#include "../Utility/perlin.h"

typedef struct {
    perlin* perlin;
} noise_texture;

noise_texture* noise_create();

#endif