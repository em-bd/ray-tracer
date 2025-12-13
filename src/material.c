#include "material.h"

/* local methods */

/**
 * Schlick's approximation for reflectance:
 */
double reflectance(double cosine, double refraction_index) {
    double r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
    r0 = r0*r0;
    return r0 + (1.0 - r0) * pow((1.0 - cosine), 5);
}

/* public methods */

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
 * Create a dielectric:
 */
dielectric dielectric_create(double refraction_index) {
    dielectric d;
    d.refraction_index = refraction_index;
    return d;
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
    
    vec3 reflected = reflect(vec3_unit(r_in.dir), rec.normal);
    *scattered = ray_create(rec.p, vec3_add(reflected, vec3_scalar(random_unit_vector(), m.fuzz)));
    *attenuation = m.albedo;
    return (vec3_dot(scattered->dir, rec.normal) > 0);
}

/**
 * Scatter for a dielectric material:
 */
bool dielectric_scatter(ray r_in, hit_record rec, color* attenuation, ray* scattered, material* mat) {
    dielectric d = *((dielectric*) mat->data);

    *attenuation = vec3_create(1.0, 1.0, 1.0);
    double ri = rec.front_face ? (1.0 / d.refraction_index) : d.refraction_index;

    vec3 unit_dir = vec3_unit(r_in.dir);
    double cos_theta = fmin(vec3_dot(vec3_negative(unit_dir), rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0;
    vec3 dir;

    if (cannot_refract || reflectance(cos_theta, ri) > rand_double())
        dir = reflect(unit_dir, rec.normal);
    else
        dir = refract(unit_dir, rec.normal, ri);

    *scattered = ray_create(rec.p, dir);
    return true;
}

scatter_fn scatter_func[3] = {
    lambertian_scatter,
    metal_scatter,
    dielectric_scatter,
};