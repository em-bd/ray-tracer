#include "metal.h"

/**
 * Create a metal:
 */
metal* metal_create(color albedo, double fuzz) {
    metal* m = malloc(sizeof(metal));
    if (m == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    m->albedo = albedo;
    m->fuzz = fuzz;
    return m;
}