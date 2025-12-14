#ifndef CHECKERED_H
#define CHECKERED_H

#include "texture.h"
#include "solid_color.h"

typedef struct {
    double inv_scale;
    texture* even;
    texture* odd;
} checkered;

texture* checkered_create(double, texture*, texture*);

texture* checkered_create_from_solids(double, color, color);

#endif