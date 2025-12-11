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