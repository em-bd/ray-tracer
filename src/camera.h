#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "object.h"

#define infinity INFINITY

typedef struct {
    double aspect_ratio;
    int image_width;
    int image_height;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
} camera;

void initialize(camera*);

color ray_color(ray);

#endif