#include "dielectric.h"

/**
 * Create a dielectric:
 */
dielectric* dielectric_create(double refraction_index) {
    dielectric* d = malloc(sizeof(dielectric));
    if (d == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    d->refraction_index = refraction_index;
    return d;
}