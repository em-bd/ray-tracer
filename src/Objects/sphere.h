#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

/**
 * Sphere data structure:
 */
typedef struct {
    ray center;
    double radius;
    material* mat;
    aabb bbox;
} sphere;

object* sphere_create(vec3, double, material*);

object* moving_sphere_create(point3, point3, double, material*);

#endif