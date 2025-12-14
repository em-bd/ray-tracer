#include "sphere.h"

/**
 * Create a sphere:
 */
sphere* sphere_create(vec3 center, double radius, material* mat) {
    sphere* s = malloc(sizeof(sphere));
    if (s == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    s->center = ray_create(center, vec3_create(0, 0, 0));
    s->radius = radius;
    s->mat = mat;
    
    return s;
}

sphere* moving_sphere_create(point3 center1, point3 center2, double radius, material* mat) {
    sphere* s = malloc(sizeof(sphere));
    if (s == NULL) {
        perror("Malloc error");
        exit(1);
    }
    s->center = ray_create(center1, vec3_sub(center2, center1));
    s->radius = radius;
    s->mat = mat;

    return s;
}