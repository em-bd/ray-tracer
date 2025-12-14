#include "metal.h"

/**
 * Scatter for a metal material:
 */
bool metal_scatter(ray r_in, hit_record rec, color* attenuation, ray* scattered, void* mat) {
    metal m = *((metal*) ((material*) mat)->data);
    
    vec3 reflected = reflect(vec3_unit(r_in.dir), rec.normal);
    *scattered = ray_create_time(rec.p, vec3_add(reflected, vec3_scalar(random_unit_vector(), m.fuzz)), r_in.tm);
    *attenuation = m.albedo;
    return (vec3_dot(scattered->dir, rec.normal) > 0);
}

/**
 * Create a metal:
 */
material* metal_create(color albedo, double fuzz) {
    metal* m = malloc(sizeof(metal));
    if (m == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    m->albedo = albedo;
    m->fuzz = fuzz;

    return material_create(metal_type, m, metal_scatter);
}