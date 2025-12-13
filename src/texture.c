#include "texture.h"

/**
 * Create a texture:
 */
texture* texture_create(texture_type type, void* data) {
    texture* t = malloc(sizeof(texture));
    t->data = data;
    t->type = type;

    return t;
}

/**
 * Create a solid color texture:
 */
texture* solid_create(double r, double g, double b) {
    solid_color* s = malloc(sizeof(solid_color));
    if (s == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    s->albedo = vec3_create(r, g, b);

    return texture_create(solid_tex, s);
}

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

    return texture_create(checkered_tex, c);
}

/**
 * Create a checkered texture from two colors:
 */
texture* checkered_create_from_solids(double scale, color evens, color odds) {
    texture* even = solid_create(evens.x, evens.y, evens.z);
    texture* odd = solid_create(odds.x, odds.y, odds.z);

    return checkered_create(scale, even, odd);
}

/**
 * Get color value for the solid:
 */
color solid_value(texture* t, double a, double b, point3 p) {
    return ((solid_color*) t->data)->albedo;
}

/**
 * Get color value for the checkered texture:
 */
color checkered_value(texture* t, double u, double v, point3 p) {
    checkered* c = (checkered*) t->data;

    int xInt = (int) floor(c->inv_scale * p.x);
    int yInt = (int) floor(c->inv_scale * p.y);
    int zInt = (int) floor(c->inv_scale * p.z);

    bool isEven = (xInt + yInt + zInt) % 2 == 0;

    return isEven ? value_func[c->even->type](c->even, u, v, p) : value_func[c->odd->type](c->odd, u, v, p);
}

value_fn value_func[2] = {
    solid_value,
    checkered_value,
};