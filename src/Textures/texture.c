#include "texture.h"

/**
 * Create a texture:
 */
texture* texture_create(texture_type type, void* data, value_fn value) {
    texture* t = malloc(sizeof(texture));
    t->data = data;
    t->type = type;
    t->value = value;

    return t;
}