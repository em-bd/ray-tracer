#ifndef SOLID_COLOR_H
#define SOLID_COLOR_H

#include "texture.h"

typedef struct {
    color albedo;
} solid_color;

texture* solid_create(double, double, double);

#endif