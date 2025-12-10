#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>
#include <math.h>

#include "ray.h"

typedef struct {
    point3 p;
    vec3 normal;
    double t;
    bool front_face;
} hit_record;

typedef struct {
    point3 center;
    double radius;
} sphere;

typedef struct {
    vec3 a, b, c;
    vec3 ta, tb, tc;
    vec3 n;
} triangle;

double hit_sphere(ray, sphere);

bool hitSphere(ray*, double, double, hit_record*, sphere*);

void set_face_normal(ray*, vec3*, hit_record*);

#endif