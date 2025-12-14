#ifndef TEXTURE_H
#define TEXTURE_H

#include "../Utility/utils.h"
#include "../Utility/color.h"

typedef color (*value_fn)(void*, double*, double*, point3*);

typedef enum {
    solid_tex,
    checkered_tex,
    image_tex,
    noise_tex,
} texture_type;

typedef struct {
    void* data;
    texture_type type;
    value_fn value;
} texture;

texture* texture_create(texture_type, void*, value_fn);

#endif