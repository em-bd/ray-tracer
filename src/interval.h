#ifndef INTERVAL_H
#define INTERVAL_H

#include <stdbool.h>
#include <math.h>
#include <stdint.h>

typedef struct {
    double min;
    double max;
} interval;

interval interval_create(double, double);

double size(interval);

bool contains(interval, double);

bool surrounds(interval, double);

double clamp(interval, double);

#endif