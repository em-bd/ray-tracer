#include "dielectric.h"

/**
 * Schlick's approximation for reflectance:
 */
double reflectance(double cosine, double refraction_index) {
    double r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
    r0 = r0*r0;
    return r0 + (1.0 - r0) * pow((1.0 - cosine), 5);
}

/**
 * Scatter for a dielectric material:
 */
bool dielectric_scatter(ray r_in, hit_record rec, color* attenuation, ray* scattered, void* mat) {
    dielectric d = *((dielectric*) ((material*) mat)->data);

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

    *scattered = ray_create_time(rec.p, dir, r_in.tm);
    return true;
}

/**
 * Create a dielectric:
 */
material* dielectric_create(double refraction_index) {
    dielectric* d = malloc(sizeof(dielectric));
    if (d == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    d->refraction_index = refraction_index;

    return material_create(dielectric_type, d, dielectric_scatter);
}