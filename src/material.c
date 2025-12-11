#include "material.h"

/**
 * Create a lambertian:
 */
lambertian lambertian_create(double x, double y, double z) {
    lambertian l;
    l.albedo = vec3_create(x, y, z);
    return l;
}

/**
 * Create a metal:
 */
metal metal_create(double x, double y, double z, double fuzz) {
    metal m;
    m.albedo = vec3_create(x, y, z);
    m.fuzz = fuzz;
    return m;
}

/**
 * Scatter for a lambertian:
 */
bool lambertian_scatter(ray r_in, hit_record rec, color* attenuation, ray* scattered, material* mat) {
    lambertian l = *((lambertian*) mat->data);
    
    vec3 scatter_dir = vec3_add(rec.normal, random_unit_vector());

    // catch degenerate scatter direction
    if (near_zero(scatter_dir))
        scatter_dir = rec.normal;

    *scattered = ray_create(rec.p, scatter_dir);
    *attenuation = l.albedo;
    return true;
}

/**
 * Scatter for a metal material:
 */
bool metal_scatter(ray r_in, hit_record rec, color* attenuation, ray* scattered, material* mat) {
    metal m = *((metal*) mat->data);
    
    vec3 reflected = reflect(r_in.dir, rec.normal);
    // account for "fuzzy" metals:
    reflected = vec3_add(vec3_unit(reflected), (vec3_scalar(random_unit_vector(), m.fuzz)));

    *scattered = ray_create(rec.p, reflected);
    *attenuation = m.albedo;
    return (vec3_dot(scattered->dir, rec.normal) > 0);
}

scatter_fn scatter_func[2] = {
    lambertian_scatter,
    metal_scatter,
};