#include "solid_color.h"

/**
 * Get color value for the solid:
 */
color solid_value(void* t, double* a, double* b, point3* p) {
    return ((solid_color*) ((texture*) t)->data)->albedo;
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

    return texture_create(solid_tex, s, solid_value);
}