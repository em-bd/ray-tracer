#include "ray.h"

/**
 * Compute a point along the ray:
 */
point3 at(ray r, double t) {
    return vec3_add(r.orig, vec3_scalar(r.dir, t));
}

ray ray_create(vec3 orig, vec3 dir) {
    ray r;
    r.orig = orig;
    r.dir = dir;
    return r;
}