#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>

#include "utils.h"

/**
 * Object Type enumerated data structure:
 */
typedef enum {
    sphere_obj,
    triangle_obj,
} obj_type;

/**
 * Object data structure:
 */
typedef struct {
    void* data;
    obj_type type;
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
    vec3 n;
} triangle;

typedef bool (*hit_fn)(ray, interval, hit_record*, object*);

bool hit(ray, interval, hit_record*, object**);

bool hit_sphere(ray, interval, hit_record*, object*);

bool hit_triangle(ray, interval, hit_record*, object*);

extern hit_fn hit_func[2];

void set_face_normal(ray, vec3, hit_record*);

#endif