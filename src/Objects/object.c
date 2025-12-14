#include "object.h"

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
    get_sphere_uv(outward_normal, &rec->u, &rec->v);
    rec->mat = ((sphere*) o->data)->mat;

    return true;
}

/**
 * Determine if the ray hits the triangle:
 */
bool hit_triangle(ray r, interval i, hit_record* rec, object* o) {
    triangle* tri = (triangle*) o->data;

    // ray-plane intersection:
    vec3 e1 = vec3_sub(tri->b, tri->a);
    vec3 e2 = vec3_sub(tri->c, tri->a);
    vec3 norm = vec3_cross(e1, e2);

    double denom = vec3_dot(r.dir, norm);
    // parallel to plane:
    if (fabs(denom) < epsilon)  return false;

    double t = vec3_dot(vec3_sub(tri->a, r.orig), norm) / denom;
    if (!surrounds(i, t))   return false;

    // parameterize ray:
    point3 p = at(r, t);

    // barycentric coordinate calculations:
    vec3 n_a = vec3_cross(vec3_sub(tri->c, tri->b), vec3_sub(p, tri->b));
    vec3 n_b = vec3_cross(vec3_sub(tri->a, tri->c), vec3_sub(p, tri->c));
    vec3 n_c = vec3_cross(vec3_sub(tri->b, tri->a), vec3_sub(p, tri->a));
    vec3 n = vec3_cross(vec3_sub(tri->b, tri->a), vec3_sub(tri->c, tri->a));

    double x = vec3_dot(n, n_a) / vec3_dot(n, n);
    double y = vec3_dot(n, n_b) / vec3_dot(n, n);
    double z = vec3_dot(n, n_c) / vec3_dot(n, n);

    // any barycentrics are negative, no hit:
    if (x < -epsilon || y < -epsilon || z < -epsilon) return false;

    rec->t = t;
    rec->p = p;
    vec3 outward_normal = vec3_unit(norm);
    set_face_normal(r, outward_normal, rec);
    rec->mat = tri->mat;
    rec->u = x;
    rec->v = y;

    return true;
}

/**
 * Determine if the ray hits the quadrilateral:
 */
bool hit_quad(ray r, interval i, hit_record* rec, object* o) {
    quad* q = (quad*) o->data;
    double denom = vec3_dot(q->normal, r.dir);

    // doesn't hit if the ray is parallel:
    if (fabs(denom) < 1e-8) return false;

    // doesn't hit if the point parameter t is outside the ray interval:
    double t = (q->D - vec3_dot(q->normal, r.orig)) / denom;
    if (!contains(i, t))    return false;

    // determine if the hit point is within the planar shape:
    point3 intersection = at(r, t);
    vec3 planar_hitpt_vec = vec3_sub(intersection, q->Q);
    double alpha = vec3_dot(q->w, vec3_cross(planar_hitpt_vec, q->v));
    double beta = vec3_dot(q->w, vec3_cross(q->u, planar_hitpt_vec));

    if (!is_interior(alpha, beta, rec))  return false;

    rec->t = t;
    rec->p = intersection;
    rec->mat = q->mat;
    set_face_normal(r, q->normal, rec);

    return true;
}