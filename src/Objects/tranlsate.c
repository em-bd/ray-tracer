#include "translate.h"

// PRIVATE FUNCTIONS

/**
 * Determine if a ray has hit this translated object:
 */
hit_fn hit_translate(ray r, interval in, hit_record* rec, void* o) {
    translate* t = ((translate*) ((object*) o)->data);

    // move the ray backwards by the offset:
    ray offset_r = ray_create(vec3_sub(r.orig, t->offset), r.dir, r.tm);

    // determine whether an intersection exists along the offset of the ray
    if (!t->object->hit(offset_r, in, rec, t->object))
        return false;

    // move the intersection point forwards by the offset
    rec->p = vec3_add(rec->p, t->offset);
    return true;
}

aabb translate_box(translate* t) {
    return aabb_offset(t->object->bbox, t->offset);
}

// PUBLIC FUNCTIONS

/**
 * Create a translated object:
 */
object* translate_create(object* o, vec3 offset) {
    translate* t = malloc(sizeof(translate));
    if (t == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    t->object = o;
    t->offset = offset;

    return object_create(translate_obj, t, translate_box(t), hit_translate);
}