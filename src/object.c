#include "object.h"

/**
 * Determine if the ray hits the sphere:
 */
bool hitSphere(ray r, interval i, hit_record* rec, sphere s) {
    vec3 oc = vec3_sub(s.center, r.orig);
    double a = length_sqd(r.dir);
    double h = vec3_dot(r.dir, oc);
    double c = length_sqd(oc) - s.radius * s.radius;

    double discriminant = h*h - a*c;
    if (discriminant < 0)
        return false;

    double sqrtd = sqrt(discriminant);

    double root = (h - sqrtd) / a;
    // check if the root is in the acceptable range:
    if (root <= i.min || i.max <= root) {
        root = (h + sqrtd) / a;
        if (root <= i.min || i.max <= root)
            return false;
    }

    // update hit record:
    rec->t = root;
    rec->p = at(r, rec->t);
    vec3 outward_normal = vec3_scalar(vec3_sub(rec->p, s.center), (1.0 / s.radius));
    set_face_normal(r, outward_normal, rec);

    return true;
}

/**
 * Sets the normal to face outward from the surface:
 */
void set_face_normal(ray r, vec3 outward_normal, hit_record* rec) {
    rec->front_face = vec3_dot(r.dir, outward_normal) < 0;
    rec->normal = rec->front_face ? outward_normal : vec3_negative(outward_normal);
}