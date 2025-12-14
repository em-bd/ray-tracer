#include "emissive.h"

/**
 * Scatter function for an emissive material:
 */
bool emissive_scatter(ray r_in, hit_record rec, color* attenuation, ray* scattered, void* mat) {
    return false;
}

/**
 * Create an emissive material:
 */
material* emissive_create(texture* t) {
    emissive* e = malloc(sizeof(emissive));
    if (e == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    e->tex = t;

    return material_create(emissive_type, e, emissive_scatter);
}

/**
 * Create an emissive material from a color:
 */
material* emissive_create_color(color c) {
    return emissive_create(solid_create(c.x, c.y, c.z));
}

/**
 * Determine what color is emitted from this emissive material:
 */
color emitted(emissive* e, double* u, double* v, point3* p) {
    return e->tex->value(e->tex, u, v, p);
}