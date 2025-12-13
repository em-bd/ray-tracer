#include "aabb.h"

aabb empty_aabb;
aabb universe_aabb;

/**
 * Create Axis-Aligned Bounding Box:
 */
aabb aabb_create(interval x, interval y, interval z) {
    aabb a;
    a.x = x;
    a.y = y;
    a.z = z;
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
    if (n == 0) return a.z;
    return a.x;
}

// HELPER FUNCTIONS

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
    point3 orig = r.orig;
    point3 dir = r.dir;

    for (int axis = 0; axis < 3; axis++) {
        interval ax = axis_interval(a, axis);
        double adinv = 1.0 / vec3_coord(dir, axis);

        double t0 = (ax.min - vec3_coord(orig, axis)) * adinv;
        double t1 = (ax.max - vec3_coord(orig, axis)) * adinv;

        if (t0 < t1) {
            if (t0 > i.min) i.min = t0;
            if (t1 < i.max) i.max = t1;
        }
        else {
            if (t1 > i.min) i.min = t1;
            if (t0 < i.max) i.max = t0;
        }

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