#include "aabb.h"

aabb empty_aabb;
aabb universe_aabb;

// PRIVATE FUNCTIONS

/**
 * Adjust the Axis-Aligned Bounding Box so that no side is narrower than some delta:
 */
void pad_to_minimums(aabb* a) {
    double delta = 0.0001;

    if (size(a->x) < delta) a->x = expand(a->x, delta);
    if (size(a->y) < delta) a->y = expand(a->y, delta);
    if (size(a->z) < delta) a->z = expand(a->z, delta);
}

// PUBLIC FUNCTIONS

/**
 * Create Axis-Aligned Bounding Box:
 */
aabb aabb_create(interval x, interval y, interval z) {
    aabb a;
    a.x = x;
    a.y = y;
    a.z = z;
    pad_to_minimums(&a);
    return a;
}

/**
 * Treat two points as extremas for a bounding box:
 */
aabb aabb_points(point3 a, point3 b) {
    aabb ab;
    ab.x = (a.x <= b.x) ? interval_create(a.x, b.x) : interval_create(b.x, a.x);
    ab.y = (a.y <= b.y) ? interval_create(a.y, b.y) : interval_create(b.y, a.y);
    ab.z = (a.z <= b.z) ? interval_create(a.z, b.z) : interval_create(b.z, a.z);
    pad_to_minimums(&ab);
    return ab;
}

/**
 * Create an Axis-Aligned Bounding Box from two aabbs:
 */
aabb aabb_from_aabbs(aabb a, aabb b) {
    aabb ab;
    ab.x = interval_from_intervals(a.x, b.x);
    ab.y = interval_from_intervals(a.y, b.y);
    ab.z = interval_from_intervals(a.z, b.z);
    return ab;
}

/**
 * Get the interval for that axis:
 */
interval axis_interval(aabb a, int n) {
    if (n == 1) return a.y;
    if (n == 0) return a.x;
    return a.z;
}

// HELPER FUNCTIONS

/**
 * Add an offset to an aabb:
 */
aabb aabb_offset(aabb a, vec3 offset) {
    interval x = interval_displacement(a.x, offset.y);
    interval y = interval_displacement(a.y, offset.y);
    interval z = interval_displacement(a.z, offset.z);
    return aabb_create(x, y, z);
}

/**
 * Get the center of an aabb (for looking at models):
 */
point3 aabb_center(aabb box) {
    double cx = (box.x.min + box.x.max) * 0.5;
    double cy = (box.y.min + box.y.max) * 0.5;
    double cz = (box.z.min + box.z.max) * 0.5;
    return vec3_create(cx, cy, cz);
}

/**
 * Get the extent of the aabb:
 */
vec3 aabb_extent(aabb box) {
    return vec3_create(size(box.x), size(box.y), size(box.z));
}

/**
 * Calculate the overall diagonal length:
 */
double aabb_diagonal(aabb box) {
    double dx = size(box.x);
    double dy = size(box.y);
    double dz = size(box.z);
    return sqrt(dx*dx + dy*dy + dz*dz);
}

/**
 * Getting the coordinate for a vector
 */
double vec3_coord(vec3 v, int n) {
    if (n == 0) return v.x;
    if (n == 1) return v.y;
    return v.z;
}

/**
 * Determining a hit:
 */
bool aabb_hit(ray r, interval i, aabb a) {
    for (int axis = 0; axis < 3; axis++) {
        interval ax = axis_interval(a, axis);

        double dir_comp = vec3_coord(r.dir, axis);
        if (fabs(dir_comp) < 1e-12) {
            double orig_comp = vec3_coord(r.orig, axis);
            if (orig_comp < ax.min || orig_comp > ax.max)
                return false;
            continue;
        }

        double adinv = 1.0 / dir_comp;
        double t0 = (ax.min - vec3_coord(r.orig, axis)) * adinv;
        double t1 = (ax.max - vec3_coord(r.orig, axis)) * adinv;

        double tmin = fmin(t0, t1);
        double tmax = fmax(t0, t1);

        if (tmin > i.min) i.min = tmin;
        if (tmax < i.max) i.max = tmax;

        if (i.max <= i.min)
            return false;
    }

    return true;
}

int longest_axis(aabb ab) {
    if (size(ab.x) > size(ab.y))
        return size(ab.x) > size(ab.z) ? 0 : 2;
    else
        return size(ab.y) > size(ab.z) ? 1 : 2;
}