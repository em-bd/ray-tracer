#ifndef VECTORS_H
#define VECTORS_H

#include "utils.h"

typedef struct {
    double x;
    double y;
    double z;
} vec3;

typedef vec3 point3;

vec3 vec3_create(double, double, double);

double vec3_dot(vec3, vec3);

vec3 vec3_cross(vec3, vec3);

vec3 vec3_sub(vec3, vec3);

vec3 vec3_add(vec3, vec3);

vec3 vec3_mul(vec3, vec3);

vec3 vec3_scalar(vec3, double);

vec3 vec3_unit(vec3);

vec3 vec3_normalize(vec3);

vec3 random_in_unit_disk();

vec3 vec3_negative(vec3);

vec3 vec3_rand();

vec3 vec3_random(interval);

vec3 random_unit_vector();

vec3 random_on_hemisphere(vec3);

vec3 reflect(vec3, vec3);

vec3 refract(vec3, vec3, double);

double vec3_length(vec3);

double length_sqd(vec3);

bool near_zero(vec3);

#endif