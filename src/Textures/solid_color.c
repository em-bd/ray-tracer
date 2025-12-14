#include "solid_color.h"

/**
 * Create a solid color texture:
 */
solid_color* solid_create(double r, double g, double b) {
    solid_color* s = malloc(sizeof(solid_color));
    if (s == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    s->albedo = vec3_create(r, g, b);

    return s;
}