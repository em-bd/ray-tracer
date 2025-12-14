#ifndef BVH_NODE_H
#define BVH_NODE_H

#include <stdio.h>
#include "object.h"

/**
 * Bounding Volume Hierarchy Node data structure:
 */
typedef struct {
    object* left;
    object* right;
    aabb bbox;
} bvh_node;

typedef bool (*hit_fn)(ray, interval, hit_record*, object*);
extern hit_fn hit_func[NUM_OBJ_TYPES];

bvh_node* bvh_node_create(object*, object*);

object* build_bvh(object**, size_t, size_t);

void free_objects(object*);

#endif