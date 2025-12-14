#ifndef MATERIAL_H
#define MATERIAL_H

#include "../Utility/ray.h"
#include "metal.h"
#include "lambertian.h"
#include "dielectric.h"
#include "emissive.h"

#define NUM_SCATTER_TYPES       4

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
 * Material data structure:
 */
typedef struct {
    void* data;
    scatter_type type;
} material;

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

extern scatter_fn scatter_func[NUM_SCATTER_TYPES];

#endif