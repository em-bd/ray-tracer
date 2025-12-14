#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdio.h>
#include <stdlib.h>

#include "../Utility/ray.h"
#include "../Utility/color.h"
#include "../Textures/texture.h"

/**
 * Scatter type enumeration structure:
 */
typedef enum {
    lambertian_type,
    metal_type,
    dielectric_type,
    emissive_type,
} scatter_type;

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
    void* mat;
} hit_record;

typedef bool (*scatter_fn)(ray, hit_record, color*, ray*, void*);

/**
 * Material data structure:
 */
typedef struct {
    void* data;
    scatter_type type;
    scatter_fn scatter;
} material;

material* material_create(scatter_type, void*, scatter_fn);

#endif