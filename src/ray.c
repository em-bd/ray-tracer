#include "ray.h"

/**
 * Compute a point along the ray
 */
point3 at(ray r, double t) {
    return vec3_add(r.orig, vec3_scalar(r.dir, t));
}