#ifndef INTERVAL_H
#define INTERVAL_H

#include <stdbool.h>
#include <math.h>
#include <stdint.h>

typedef struct {
    double min;
    double max;
} interval;

extern interval empty, universe;

interval interval_create(double, double);

interval interval_from_intervals(interval, interval);

interval expand(interval, double);

double size(interval);

bool contains(interval, double);

bool surrounds(interval, double);

double clamp(interval, double);

#endif