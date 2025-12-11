#include "object.h"

/**
 * Take a list of objects and determine which hits
 */
bool hit(ray r, interval in, hit_record* rec, object** objects) {
    hit_record temp;
    bool hit_anything = false;
    double closest_so_far = in.max;

    // iterate through object list to find the closest one:
    for (int i = 0; objects[i] != NULL; i++) {
        object* o = objects[i];
        interval int1 = interval_create(in.min, closest_so_far);
        if (hit_func[o->type](r, int1, &temp, o)) {
            hit_anything = true;
            closest_so_far = temp.t;
            *rec = temp;
        }
    }

    return hit_anything;
}

/**
 * Determine if the ray hits the sphere:
 */
bool hit_sphere(ray r, interval i, hit_record* rec, object* o) {
    sphere* s = (sphere *) o->data;
    vec3 oc = vec3_sub(s->center, r.orig);
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
    vec3 outward_normal = vec3_scalar(vec3_sub(rec->p, s->center), (1.0 / s->radius));
    set_face_normal(r, outward_normal, rec);
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

/**
 * Hit function array declaration:
 */
hit_fn hit_func[2] = {
    hit_sphere,
    hit_triangle,
};

/**
 * Sets the normal to face outward from the surface:
 */
void set_face_normal(ray r, vec3 outward_normal, hit_record* rec) {
    rec->front_face = vec3_dot(r.dir, outward_normal) < 0;
    rec->normal = rec->front_face ? outward_normal : vec3_negative(outward_normal);
}

sphere sphere_create(vec3 center, double radius, material* mat) {
    sphere s;
    s.center = center;
    s.radius = radius;
    s.mat = mat;
    return s;
}