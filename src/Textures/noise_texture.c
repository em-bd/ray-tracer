#include "noise_texture.h"

/**
 * Create a noise texture:
 */
noise_texture* noise_create(double scale) {
    noise_texture* noise_tex = malloc(sizeof(noise_texture));
    if (noise_tex == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    noise_tex->perlin = perlin_create();
    noise_tex->scale = scale;
    
    return noise_tex;
}