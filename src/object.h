#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>

#include "utils.h"

/**
 * Object data structure:
 */
typedef struct {
    void* data;
    hit_func hit;
} object;

/**
 * Hit record data structure:
 */
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

typedef bool (*hit_func)(object*, ray, interval, hit_record);

bool hitSphere(ray, interval, hit_record*, sphere);

void set_face_normal(ray, vec3, hit_record*);

#endif