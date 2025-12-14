#include "material.h"

// PUBLIC METHODS

/**
 * Create a material:
 */
material* material_create(scatter_type type, void* data, scatter_fn scatter) {
    material* m = malloc(sizeof(material));
    if (m == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    m->type = type;
    m->data = data;
    m->scatter = scatter;
    return m;
}