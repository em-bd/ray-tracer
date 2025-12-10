#include "vectors.h"


/**
 * Create a vector with coordiantes:
 */
vec3 vec3_create(double x, double y, double z) {
    vec3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}
/** VECTOR MATH **/

/**
 * Dot product between two vectors
 */
double vec3_dot(vec3 v, vec3 w) {
    return v.x * w.x + v.y * w.y + v.z * w.z;
}

/**
 * Cross product between two vectors
 */
vec3 vec3_cross(vec3 v, vec3 w) {
    return vec3_create( 
        v.y * w.z - w.y * v.z,
        w.x * v.z - v.x * w.z,
        v.x * w.y - v.y * w.x);
}

/**
 * Vector subtraction
 */
vec3 vec3_sub(vec3 v, vec3 w) {
    return vec3_create(v.x - w.x, v.y - w.y, v.z - w.z);
}

/**
 * Vector addition
 */
vec3 vec3_add(vec3 v, vec3 w) {
    return vec3_create(v.x + w.x, v.y + w.y, v.z + w.z);
}

/**
 * Multiple a vector by a scalar
 */
vec3 vec3_scalar(vec3 v, double s) {
    return vec3_create(v.x * s, v.y * s, v.z * s);
}

/**
 * Unit vector
 */
vec3 vec3_unit(vec3 v) {
    return vec3_scalar(v, (1.0 / vec3_length(v)));
}

/**
 * Negative:
 */
vec3 vec3_negative(vec3 v) {
    return vec3_create(-v.x, -v.y, -v.z);
}

double length_sqd(vec3 v) {
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

/**
 * Length of the vector
 */
double vec3_length(vec3 v) {
    return sqrt(length_sqd(v));
}