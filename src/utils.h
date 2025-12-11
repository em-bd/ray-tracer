#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <stdbool.h>

// common headers
#include "ray.h"
#include "color.h"

// macros
#define infinity    INFINITY
#define pi          3.1415926535897932385

typedef struct {
    double min;
    double max;
} interval;

interval interval_create(double, double);

double degrees_to_radians(double);

double rand_double();

double random_double(interval);

double size(interval);

bool contains(interval, double);

bool surrounds(interval, double);

double clamp(interval, double);

#endif