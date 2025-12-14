#include "ray.h"

/**
 * Compute a point along the ray:
 */
point3 at(ray r, double t) {
    return vec3_add(r.orig, vec3_scalar(r.dir, t));
}

/**
 * Create a ray with time information:
 */
ray ray_create_time(vec3 orig, vec3 dir, double time) {
    ray r;
    r.orig = orig;
    r.dir = dir;
    r.tm = time;
    return r;
}

/**
 * Create a ray:
 */
ray ray_create(vec3 orig, vec3 dir) {
    // create a ray with 0 as the time info:
    return ray_create_time(orig, dir, 0);
}