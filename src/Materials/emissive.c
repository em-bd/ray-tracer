#include "emissive.h"

/**
 * Create an emissive material:
 */
emissive* emissive_create(texture* t) {
    emissive* e = malloc(sizeof(emissive));
    if (e == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    e->tex = t;

    return e;
}

/**
 * Create an emissive material from a color:
 */
emissive* emissive_create_color(color c) {
    return emissive_create(texture_create(solid_tex, solid_create(c.x, c.y, c.z)));
}

/**
 * Determine what color is emitted from this emissive material:
 */
color emitted(emissive* e, double* u, double* v, point3* p) {
    return value_func[e->tex->type](e->tex, u, v, p);
}