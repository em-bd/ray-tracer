#include "object.h"

// PRIVATE FUNCTIONS

void get_sphere_uv(point3 p, double u, double v) {
    double theta = acos(-p.y);
    double phi = atan2(-p.z, p.x) + pi;

    u = phi / (2 * pi);
    v = theta / pi;
}

/**
 * Sets the normal to face outward from the surface:
 */
void set_face_normal(ray r, vec3 outward_normal, hit_record* rec) {
    rec->front_face = vec3_dot(r.dir, outward_normal) < 0;
    rec->normal = rec->front_face ? outward_normal : vec3_negative(outward_normal);
}

/**
 * Determine if the ray hits the sphere:
 */
bool hit_sphere(ray r, interval i, hit_record* rec, object* o) {
    sphere* s = (sphere *) o->data;
    point3 current_center = at(s->center, r.tm);
    vec3 oc = vec3_sub(current_center, r.orig);
    double a = length_sqd(r.dir);
    double h = vec3_dot(r.dir, oc);
    double c = length_sqd(oc) - s->radius * s->radius;

    double discriminant = h*h - a*c;
    if (discriminant < 0)
        return false;

    double sqrtd = sqrt(discriminant);

    double root = (h - sqrtd) / a;
    // check if the root is in the acceptable range:
    if (!surrounds(i, root)) {
        root = (h + sqrtd) / a;
        if (!surrounds(i, root))
            return false;
    }

    // update hit record:
    rec->t = root;
    rec->p = at(r, rec->t);
    vec3 outward_normal = vec3_scalar(vec3_sub(rec->p, current_center), (1.0 / s->radius));
    set_face_normal(r, outward_normal, rec);
    get_sphere_uv(outward_normal, rec->u, rec->v);
    rec->mat = ((sphere*) o->data)->mat;

    return true;
}

/**
 * Determine if the ray hits the triangle:
 */
bool hit_triangle(ray r, interval i, hit_record* rec, object* o) {
    //TODO: complete triangle hit calculation
    return true;
}