#ifndef TEXTURE_H
#define TEXTURE_H

#include "../Utility/color.h"
#include "image.h"
#include "solid_color.h"
#include "noise_texture.h"

#define NUM_TEX_TYPES       4

typedef enum {
    solid_tex,
    checkered_tex,
    image_tex,
    noise_tex,
} texture_type;

typedef struct {
    void* data;
    texture_type type;
} texture;

typedef struct {
    double inv_scale;
    texture* even;
    texture* odd;
} checkered;

texture* texture_create(texture_type, void*);

texture* checkered_create(double, texture*, texture*);

texture* checkered_create_from_solids(double, color, color);

typedef color (*value_fn)(texture*, double*, double*, point3*);
extern value_fn value_func[NUM_TEX_TYPES];

#endif