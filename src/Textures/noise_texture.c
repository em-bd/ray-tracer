#include "noise_texture.h"

/**
 * Get color value for perlin noise:
 */
color noise_value(void* t, double* u, double* v, point3* p) {
    noise_texture* noise_tex = ((noise_texture*) ((texture*) t)->data);
    return vec3_scalar(vec3_create(.5, .5, .5), (1 + sin(noise_tex->scale * p->z + 10 * turb(noise_tex->perlin, p, 7))));
}

/**
 * Create a noise texture:
 */
texture* noise_create(double scale) {
    noise_texture* noise_text = malloc(sizeof(noise_texture));
    if (noise_text == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    noise_text->perlin = perlin_create();
    noise_text->scale = scale;
    
    return texture_create(noise_tex, noise_text, noise_value);
}