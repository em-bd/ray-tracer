#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>

#include "../Utility/aabb.h"
#include "../Utility/utils.h"
#include "sphere.h"
#include "triangle.h"
#include "quad.h"

#define NUM_OBJ_TYPES       4

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

object* object_create(obj_type, void*);

bool hit_sphere(ray, interval, hit_record*, object*);
bool hit_triangle(ray, interval, hit_record*, object*);

#endif