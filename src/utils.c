#include "utils.h"

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
 * Convert degrees to radians:
 */
double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
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