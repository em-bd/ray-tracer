#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

#include "interval.h"

// macros
#define infinity    INFINITY
#define pi          3.1415926535897932385

double degrees_to_radians(double);

double rand_double();

double random_double(interval);

#endif