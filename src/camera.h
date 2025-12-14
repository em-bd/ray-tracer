#ifndef CAMERA_H
#define CAMERA_H

#include "Objects/bvh_node.h"
#include "Objects/quad.h"
#include "Objects/sphere.h"
#include "Objects/triangle.h"

#include "Materials/dielectric.h"
#include "Materials/emissive.h"
#include "Materials/lambertian.h"
#include "Materials/metal.h"

#include "Textures/image.h"
#include "Textures/noise_texture.h"
#include "Textures/solid_color.h"
#include "Textures/checkered.h"

#define SAMPLES_PER_PIXEL 100

/**
 * Camera data structure:
 */
typedef struct {
    double aspect_ratio;                
    int image_width, image_height;      
    point3 center;                      
    point3 pixel00_loc;                 
    vec3 pixel_delta_v, pixel_delta_u;  
    double pixel_samples_scale;         
    int max_depth;                      
    double vfov;                        
    point3 lookfrom;                    
    point3 lookat;                      
    vec3 vup;                           
    vec3 v, u, w;                       
    double defocus_angle;
    double focus_dist;
    vec3 defocus_disk_u, defocus_disk_v;
    color background;       
} camera;

extern camera* c;
extern object* world;

void initialize(double, int, vec3, vec3, int, vec3, double);

void render();

color ray_color(ray, int);

#endif