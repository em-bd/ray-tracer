#include "interval.h"

/**
 * Interval creation:
 */
interval interval_create(double min, double max) {
    interval i;
    i.min = min;
    i.max = max;
    return i;
}

/**
 * Interval size:
 */
double size(interval i) {
    return i.max - i.min;
}

/**
 * Check if element is in interval (inclusive)
 */
bool contains(interval i, double x) {
    return i.min <= x && x <= i.max;
}

/**
 * Check if element is surrounded by interval minimum and maximum:
 */
bool surrounds(interval i, double x) {
    return i.min < x && x < i.max;
}

/**
 * Clamp helper function:
 */
double clamp(interval i, double x) {
    if (x < i.min) return i.min;
    if (x > i.max) return i.max;
    return x;
}