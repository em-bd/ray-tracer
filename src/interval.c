#include "interval.h"

interval empty;
interval universe;

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
 * Create an interval by enclosing two intervals:
 */
interval interval_from_intervals(interval i1, interval i2) {
    interval i;
    i.min = (i1.min <= i2.min) ? i1.min : i2.min;
    i.max = (i1.max >= i2.max) ? i1.max : i2.max;
    return i;
}

/**
 * Pad an interval by a given amount:
 */
interval expand(interval i, double d) {
    double padding = d/2;
    return interval_create(i.min - padding, i.max + padding);
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