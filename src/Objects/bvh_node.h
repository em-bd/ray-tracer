#ifndef BVH_NODE_H
#define BVH_NODE_H

#include "object.h"

/**
 * Bounding Volume Hierarchy Node data structure:
 */
typedef struct {
    object* left;
    object* right;
    aabb bbox;
    hit_fn* hit;
} bvh_node;

object* bvh_node_create(object*, object*);

object* build_bvh(object**, size_t, size_t);

void free_objects(object*);

#endif