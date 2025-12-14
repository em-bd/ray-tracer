#ifndef OBJECT_H
#define OBJECT_H

#include "../Utility/utils.h"
#include "../Materials/material.h"
#include "../Utility/aabb.h"

typedef bool (*hit_fn)(ray, interval, hit_record*, void*);

/**
 * Object Type enumerated data structure:
 */
typedef enum {
    sphere_obj,
    moving_sphere_obj,
    triangle_obj,
    quad_obj,
    triangle_mesh_obj,
    translate_obj,
    bvh_node_obj,
} obj_type;

/**
 * Object data structure:
 */
typedef struct {
    void* data;
    obj_type type;
    aabb bbox;
    hit_fn hit;
} object;

object* object_create(obj_type, void*, aabb, hit_fn);

#endif