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
 * Multiply element by element:
 */
vec3 vec3_mul(vec3 v, vec3 w) {
    return vec3_create(v.x * w.x, v.y * w.y, v.z * w.z);
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
 * Normalize the vector
 */
vec3 vec3_normalize(vec3 v) {
    // length is 0, return a 0 vector:
    if (vec3_length(v) == 0.0)
        return vec3_create(0,0,0);

    double inv = 1.0 / vec3_length(v);
    return vec3_create(v.x * inv, v.y * inv, v.z * inv);
}

/**
 * Generate a random point inside a unit disk:
 */
vec3 random_in_unit_disk() {
    do {
        interval i = interval_create(-1, 1);
        vec3 p = vec3_create(random_double(i), random_double(i), 0);
        if (length_sqd(p) < 1)
            return p;
    } while (1);
}

/**
 * Negative:
 */
vec3 vec3_negative(vec3 v) {
    return vec3_create(-v.x, -v.y, -v.z);
}

/**
 * Generate a random vector:
 */
vec3 vec3_rand() {
    return vec3_create(rand_double(), rand_double(), rand_double());
}

/**
 * Generate a random vector with bounds for coordinates:
 */
vec3 vec3_random(interval i) {
    return vec3_create(random_double(i), random_double(i), random_double(i));
}

/**
 * Generates a random unit vector:
 */
vec3 random_unit_vector() {
    do {
        vec3 p = vec3_random(interval_create(-1.0, 1.0));
        double lensq = length_sqd(p);
        if (1e-160 < lensq && lensq <= 1)
            return p;
    } while (true);
}

vec3 random_on_hemisphere(vec3 normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (vec3_dot(on_unit_sphere, normal) > 0.0)
        return on_unit_sphere;
    else
        return vec3_negative(on_unit_sphere);
}

/**
 * Computing reflected vector:
 */
vec3 reflect(vec3 v, vec3 n) {
    return vec3_sub(v, vec3_scalar(n, (2.0 * vec3_dot(v, n))));
}

/**
 * Computing refracted vector:
 */
vec3 refract(vec3 v, vec3 n, double etai_over_etat) {
    double cos_theta = fmin(vec3_dot(vec3_negative(v), n), 1.0);
    vec3 r_out_perp = vec3_scalar(vec3_add(v, vec3_scalar(n, cos_theta)), etai_over_etat);
    vec3 r_out_parallel = vec3_scalar(n, -sqrt(fabs(1.0 - length_sqd(r_out_perp))));
    return vec3_add(r_out_perp, r_out_parallel);
}

/**
 * Vector length helper:
 */
double length_sqd(vec3 v) {
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

/**
 * Length of the vector
 */
double vec3_length(vec3 v) {
    return sqrt(length_sqd(v));
}

/**
 * Find if the vector is close to 0 in all directions:
 */
bool near_zero(vec3 v) {
    double s = 1e-8;
    return (fabs(v.x) < s) && (fabs(v.y) < s) && (fabs(v.z) < s);
}