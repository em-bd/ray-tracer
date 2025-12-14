#include "lambertian.h"

/**
 * Create a lambertian:
 */
lambertian* lambertian_create(texture* tex) {
    lambertian* l = malloc(sizeof(lambertian));
    if (l == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    l->tex = tex;
    return l;
}