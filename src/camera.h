#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"
#include "color.h"
#include "ray.h"
#include "interval.h"

#define SAMPLES_PER_PIXEL 100

/**
 * Camera data structure:
 */
typedef struct {
    double aspect_ratio;                // Ratio of image width over height
    int image_width, image_height;      // Rendered image width and height in pixel count
    point3 center;                      // Camera center
    point3 pixel00_loc;                 // Location of pixel (0,0)
    vec3 pixel_delta_v, pixel_delta_u;  // Offset to pixel to the right and below
    double pixel_samples_scale;         // Color scale factor for a sum of pixel samples
    int max_depth;                      // Max # of ray bounces into scene
    double vfov;                        // Vertical view angle
    point3 lookfrom;                    // Point camera is looking from
    point3 lookat;                      // Point camera is looking at
    vec3 vup;                           // Camera relative "up" direction
    vec3 v, u, w;                       // Camera frame basics vectors
} camera;

extern camera* c;
extern object** objects;

void initialize();

void render();

color ray_color(ray, int, object**);

#endif