#ifndef AABB_H
#define AABB_H

#include "interval.h"
#include "ray.h"

/**
 * Axis-aligned Bounding Box data structure:
 */
typedef struct {
    interval x, y, z;
} aabb;

extern aabb empty_aabb, universe_aabb;

aabb aabb_create(interval, interval, interval);

aabb aabb_points(point3, point3);

aabb aabb_from_aabbs(aabb, aabb);

aabb aabb_offset(aabb, vec3);

interval axis_interval(aabb, int);

point3 aabb_center(aabb);

vec3 aabb_extent(aabb);

double aabb_diagonal(aabb);

bool aabb_hit(ray, interval, aabb);

int longest_axis(aabb);

#endif