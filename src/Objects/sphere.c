#include "sphere.h"

// PRIVATE FUNCTIONS

void get_sphere_uv(point3 p, double* u, double* v) {
    double theta = acos(-p.y);
    double phi = atan2(-p.z, p.x) + pi;

    *u = phi / (2 * pi);
    *v = theta / pi;
}

/**
 * Sets the normal to face outward from the surface:
 */
void sphere_set_face_normal(ray r, vec3 outward_normal, hit_record* rec) {
    rec->front_face = vec3_dot(r.dir, outward_normal) < 0;
    rec->normal = rec->front_face ? outward_normal : vec3_negative(outward_normal);
}

/**
 * Determine if the ray hits the sphere:
 */
bool hit_sphere(ray r, interval i, hit_record* rec, void* o) {
    sphere* s = ((sphere *) ((object*) o)->data);
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
    sphere_set_face_normal(r, outward_normal, rec);
    get_sphere_uv(outward_normal, &rec->u, &rec->v);
    rec->mat = s->mat;

    return true;
}

/**
 * Sphere bounding box function
 */
aabb sphere_box(sphere* s) {
    vec3 rvec = vec3_create(s->radius, s->radius, s->radius);
    return aabb_points(vec3_sub(s->center.orig, rvec), vec3_add(s->center.orig, rvec));
}

/**
 * Calculates the bbox for a moving sphere:
 */
aabb moving_sphere_box(sphere* s) {
    vec3 center0 = at(s->center, 0);
    vec3 center1 = at(s->center, 1);
    vec3 r = vec3_create(s->radius, s->radius, s->radius);

    aabb box0 = aabb_points(vec3_sub(center0, r), vec3_add(center0, r));
    aabb box1 = aabb_points(vec3_sub(center1, r), vec3_add(center1, r));

    return aabb_from_aabbs(box0, box1);
}

// PUBLIC FUNCTIONS

/**
 * Create a sphere:
 */
object* sphere_create(vec3 center, double radius, material* mat) {
    sphere* s = malloc(sizeof(sphere));
    if (s == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    s->center = ray_create(center, vec3_create(0, 0, 0));
    s->radius = radius;
    s->mat = mat;
    s->bbox = sphere_box(s);
    
    return object_create(sphere_obj, s, s->bbox, hit_sphere);
}

/**
 * Create a moving sphere:
 */
object* moving_sphere_create(point3 center1, point3 center2, double radius, material* mat) {
    sphere* s = malloc(sizeof(sphere));
    if (s == NULL) {
        perror("Malloc error");
        exit(1);
    }
    s->center = ray_create(center1, vec3_sub(center2, center1));
    s->radius = radius;
    s->mat = mat;
    s->bbox = moving_sphere_box(s);

    return object_create(moving_sphere_obj, s, s->bbox, hit_sphere);
}