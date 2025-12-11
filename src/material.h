#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "ray.h"

typedef enum {
    lambertian_type,
    metal_type,
} scatter_type;

/**
 * Material data structure:
 */
typedef struct {
    void* data;
    scatter_type type;
} material;

/**
 * Lambertian data structure:
 */
typedef struct {
    color albedo;
} lambertian;

/**
 * Metal material data structure:
 */
typedef struct {
    color albedo;
    double fuzz;
} metal;

/**
 * Hit record data structure:
 */
typedef struct {
    point3 p;
    vec3 normal;
    double t;
    bool front_face;
    material* mat;
} hit_record;

typedef bool (*scatter_fn)(ray, hit_record, color*, ray*, material*);

lambertian lambertian_create(double, double, double);

metal metal_create(double, double, double, double);

bool lambertian_scatter(ray, hit_record, color*, ray*, material*);

bool metal_scatter(ray, hit_record, color*, ray*, material*);

extern scatter_fn scatter_func[2];

#endif