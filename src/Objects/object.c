#include "object.h"

// PUBLIC FUNCTIONS

/**
 * Create an object:
 */
object* object_create(obj_type type, void* data, aabb bbox, hit_fn hit) {
    object* o = malloc(sizeof(object));
    if (o == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    o->type = type;
    o->data = data;
    o->bbox = bbox;
    o->hit = hit;
    return o;
}