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
    texture* even = texture_create(solid_tex, solid_create(evens.x, evens.y, evens.z));
    texture* odd = texture_create(solid_tex, solid_create(odds.x, odds.y, odds.z));

    return checkered_create(scale, even, odd);
}

/**
 * Get color value for the solid:
 */
color solid_value(texture* t, double* a, double* b, point3* p) {
    return ((solid_color*) t->data)->albedo;
}

/**
 * Get color value for the checkered texture:
 */
color checkered_value(texture* t, double* u, double* v, point3* p) {
    checkered* c = (checkered*) t->data;

    int xInt = (int) floor(c->inv_scale * p->x);
    int yInt = (int) floor(c->inv_scale * p->y);
    int zInt = (int) floor(c->inv_scale * p->z);

    bool isEven = (xInt + yInt + zInt) % 2 == 0;

    return isEven ? value_func[c->even->type](c->even, u, v, p) : value_func[c->odd->type](c->odd, u, v, p);
}

/**
 * Get the value for an image texture:
 */
color image_value(texture* t, double* u, double* v, point3* p) {
    image* img = (image*) t->data;
    if (img == NULL || img->image == NULL || img->image->image_height <= 0)
        return vec3_create(0, 1, 1);

    interval i0 = interval_create(0, 1);
    *u = clamp(i0, *u);
    *v = clamp(i0, *v);

    int i = (int) (*u * img->image->image_width);
    i = (i >= img->image->image_width) ? img->image->image_width - 1 : i;
    int j = (int) ((1.0 - *v) * img->image->image_height);
    j = (j >= img->image->image_height) ? img->image->image_height - 1 : j;
    const unsigned char* pixel = pixel_data(img->image, i, j);

    double color_scale = 1.0/ 255.0;
    return vec3_create(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
}

/**
 * Get color value for perlin noise:
 */
color noise_value(texture* t, double* u, double* v, point3* p) {
    noise_texture* noise_tex = ((noise_texture*) t->data);
    return vec3_scalar(vec3_create(.5, .5, .5), (1 + sin(noise_tex->scale * p->z + 10 * turb(noise_tex->perlin, p, 7))));
}

/**
 * Get color value for a texture:
 */
value_fn value_func[NUM_TEX_TYPES] = {
    solid_value,
    checkered_value,
    image_value,
    noise_value,
};