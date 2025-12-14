#ifndef SPHERE_H
#define SPHERE_H

#include "../Materials/material.h"

/**
 * Sphere data structure:
 */
typedef struct {
    ray center;
    double radius;
    material* mat;
} sphere;

sphere* sphere_create(vec3, double, material*);

sphere* moving_sphere_create(point3, point3, double, material*);

#endif