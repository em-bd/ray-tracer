#include "utils.h"

/**
 * Convert degrees to radians:
 */
double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

/**
 * Finds a random real in [0,1):
 */
double rand_double() {
    return rand() / (RAND_MAX + 1.0);
}

/**
 * Finds a random double in [min,max):
 */
double random_double(interval i) {
    return i.min + (i.max - i.min)*rand_double();
}

/*    INTERVALS       */

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