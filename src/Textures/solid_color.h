#ifndef SOLID_COLOR_H
#define SOLID_COLOR_H

#include "../Utility/utils.h"
#include "../Utility/color.h"

typedef struct {
    color albedo;
} solid_color;

solid_color* solid_create(double, double, double);

#endif