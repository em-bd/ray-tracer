#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"

#define SAMPLES_PER_PIXEL 100

/**
 * Camera data structure:
 */
typedef struct {
    double aspect_ratio;
    int image_width;
    int image_height;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    double pixel_samples_scale;
} camera;

extern camera* c;
extern object** objects;

void initialize();

void render();

color ray_color(ray, object**);

#endif