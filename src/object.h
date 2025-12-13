#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>

#include "aabb.h"
#include "material.h"

/**
 * Object Type enumerated data structure:
 */
typedef enum {
    sphere_obj,
    moving_sphere_obj,
    triangle_obj,
    bvh_node_obj,
} obj_type;

/**
 * Object data structure:
 */
typedef struct {
    void* data;
    obj_type type;
    aabb bbox;
} object;

/**
 * Bounding Volume Hierarchy Node data structure:
 */
typedef struct {
    object* left;
    object* right;
    aabb bbox;
} bvh_node;

/**
 * Sphere data structure:
 */
typedef struct {
    ray center;
    double radius;
    material* mat;
} sphere;

typedef struct {
    vec3 a, b, c;
    vec3 n;
} triangle;

typedef bool (*hit_fn)(ray, interval, hit_record*, object*);

extern hit_fn hit_func[4];

void set_face_normal(ray, vec3, hit_record*);

object* object_create(obj_type, void*);

object* moving_sphere_create(point3, point3, double, material*);

object* sphere_create(vec3, double, material*);

bvh_node bvh_node_create(object*, object*);

object* build_bvh(object**, size_t, size_t);

void free_objects(object*);

#endif