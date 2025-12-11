#ifndef COLOR_H
#define COLOR_H

#include <stdlib.h>

#include "vectors.h"
#include "utils.h"

typedef vec3 color;

const char* write_color(char* restrict, color);

#endif