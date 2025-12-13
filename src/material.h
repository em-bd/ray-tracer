#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "ray.h"
#include "texture.h"

/**
 * Scatter type enumeration structure:
 */
typedef enum {
    lambertian_type,
    metal_type,
    dielectric_type,
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
    texture* tex;
} lambertian;

/**
 * Metal material data structure:
 */
typedef struct {
    color albedo;
    double fuzz;
} metal;

/**
 * Dielectric material data structure:
 */
typedef struct {
    double refraction_index;
} dielectric;

/**
 * Hit record data structure:
 */
typedef struct {
    point3 p;
    vec3 normal;
    double t;
    bool front_face;
    double u;
    double v;
    material* mat;
} hit_record;

typedef bool (*scatter_fn)(ray, hit_record, color*, ray*, material*);

material* material_create(scatter_type, void*);

material* lambertian_create(texture*);

material* metal_create(color, double);

material* dielectric_create(double);

bool lambertian_scatter(ray, hit_record, color*, ray*, material*);

bool metal_scatter(ray, hit_record, color*, ray*, material*);

bool dielectric_scatter(ray, hit_record, color*, ray*, material*);

extern scatter_fn scatter_func[3];

#endif