#include "checkered.h"

// PRIVATE FUNCTIONS

/**
 * Get color value for the checkered texture:
 */
color checkered_value(void* t, double* u, double* v, point3* p) {
    checkered* c = ((checkered*) ((texture*) t)->data);

    int xInt = (int) floor(c->inv_scale * p->x);
    int yInt = (int) floor(c->inv_scale * p->y);
    int zInt = (int) floor(c->inv_scale * p->z);

    bool isEven = (xInt + yInt + zInt) % 2 == 0;

    return isEven ? c->even->value(c->even, u, v, p) : c->odd->value(c->odd, u, v, p);
}

// PUBLIC FUNCTIONS

/**
 * Create a checkered texture:
 */
texture* checkered_create(double scale, texture* even, texture* odd) {
    checkered* c = malloc(sizeof(checkered));
    if (c == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    c->inv_scale = 1.0 / scale;
    c->even = even;
    c->odd = odd;

    return texture_create(checkered_tex, c, checkered_value);
}

/**
 * Create a checkered texture from two colors:
 */
texture* checkered_create_from_solids(double scale, color evens, color odds) {
    texture* even = solid_create(evens.x, evens.y, evens.z);
    texture* odd = solid_create(odds.x, odds.y, odds.z);

    return checkered_create(scale, even, odd);
}
