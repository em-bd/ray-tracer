#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"

typedef enum {
    solid_tex,
    checkered_tex,
} texture_type;

typedef struct {
    void* data;
    texture_type type;
} texture;

typedef struct {
    color albedo;
} solid_color;

typedef struct {
    double inv_scale;
    texture* even;
    texture* odd;
} checkered;

texture* texture_create(texture_type, void*);

texture* solid_create(double, double, double);

texture* checkered_create(double, texture*, texture*);

texture* checkered_create_from_solids(double, color, color);

typedef color (*value_fn)(texture*, double, double, point3);
extern value_fn value_func[2];

#endif