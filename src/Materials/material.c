#include "material.h"

// PRIVATE METHODS

/**
 * Schlick's approximation for reflectance:
 */
double reflectance(double cosine, double refraction_index) {
    double r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
    r0 = r0*r0;
    return r0 + (1.0 - r0) * pow((1.0 - cosine), 5);
}

/**
 * Scatter for a lambertian:
 */
bool lambertian_scatter(ray r_in, hit_record rec, color* attenuation, ray* scattered, material* mat) {
    lambertian* l = ((lambertian*) mat->data);
    
    vec3 scatter_dir = vec3_add(rec.normal, random_unit_vector());

    // catch degenerate scatter direction
    if (near_zero(scatter_dir))
        scatter_dir = rec.normal;

    *scattered = ray_create_time(rec.p, scatter_dir, r_in.tm);
    *attenuation = value_func[l->tex->type](l->tex, &rec.u, &rec.v, &rec.p);
    return true;
}

/**
 * Scatter for a metal material:
 */
bool metal_scatter(ray r_in, hit_record rec, color* attenuation, ray* scattered, material* mat) {
    metal m = *((metal*) mat->data);
    
    vec3 reflected = reflect(vec3_unit(r_in.dir), rec.normal);
    *scattered = ray_create_time(rec.p, vec3_add(reflected, vec3_scalar(random_unit_vector(), m.fuzz)), r_in.tm);
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

    *scattered = ray_create_time(rec.p, dir, r_in.tm);
    return true;
}

// PUBLIC METHODS

scatter_fn scatter_func[NUM_SCATTER_TYPES] = {
    lambertian_scatter,
    metal_scatter,
    dielectric_scatter,
};

/**
 * Create a material:
 */
material* material_create(scatter_type type, void* data) {
    material* m = malloc(sizeof(material));
    if (m == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    m->type = type;
    m->data = data;
    return m;
}