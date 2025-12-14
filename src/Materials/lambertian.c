#include "lambertian.h"

/**
 * Scatter for a lambertian:
 */
bool lambertian_scatter(ray r_in, hit_record rec, color* attenuation, ray* scattered, void* mat) {
    lambertian* l = ((lambertian*) ((material*) mat)->data);
    
    vec3 scatter_dir = vec3_add(rec.normal, random_unit_vector());

    // catch degenerate scatter direction
    if (near_zero(scatter_dir))
        scatter_dir = rec.normal;

    *scattered = ray_create_time(rec.p, scatter_dir, r_in.tm);
    *attenuation = l->tex->value(l->tex, &rec.u, &rec.v, &rec.p);
    return true;
}

/**
 * Create a lambertian:
 */
material* lambertian_create(texture* tex) {
    lambertian* l = malloc(sizeof(lambertian));
    if (l == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    l->tex = tex;

    return material_create(lambertian_type, l, lambertian_scatter);
}