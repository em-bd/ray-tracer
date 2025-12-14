#include "quad.h"

// PRIVATE FUNCTIONS

/**
 * Sets the normal to face outward from the surface:
 */
void quad_set_face_normal(ray r, vec3 outward_normal, hit_record* rec) {
    rec->front_face = vec3_dot(r.dir, outward_normal) < 0;
    rec->normal = rec->front_face ? outward_normal : vec3_negative(outward_normal);
}

/**
 * Quadrilateral bounding box function
 */
aabb quad_box(quad* q) {
    aabb bbox_diagonal1 = aabb_points(q->Q, vec3_add(q->Q, vec3_add(q->u, q->v)));
    aabb bbox_diagonal2 = aabb_points(vec3_add(q->Q, q->u), vec3_add(q->Q, q->v));
    return aabb_from_aabbs(bbox_diagonal1, bbox_diagonal2);
}

/**
 * Determine if the hit is within the planar shape:
 */
bool is_interior(double a, double b, hit_record* rec) {
    interval unit_in = interval_create(0, 1);

    if (!contains(unit_in, a) || !contains(unit_in, b)) return false;

    // set rec u and v points
    rec->u = a;
    rec->v = b;
    return true;
}

/**
 * Determine if the ray hits the quadrilateral:
 */
bool hit_quad(ray r, interval i, hit_record* rec, void* o) {
    quad* q = ((quad*) ((object*) o)->data);
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
    quad_set_face_normal(r, q->normal, rec);

    return true;
}

/**
 * PUBLIC FUNCTIONS
 */

/**
 * Create a quadrilateral object:
 */
object* quad_create(point3 Q, vec3 u, vec3 v, material* mat) {
    quad* q = malloc(sizeof(quad));
    if (q == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    q->Q = Q;
    q->u = u;
    q->v = v;
    q->mat = mat;

    vec3 n = vec3_cross(q->u, q->v);
    q->normal = vec3_unit(n);
    q->D = vec3_dot(q->normal, q->Q);
    q->w = vec3_scalar(n, (1 / vec3_dot(n, n)));

    q->bbox = quad_box(q);

    return object_create(quad_obj, q, q->bbox, hit_quad);
}

/**
 * Create a box (array of quads):
 */
object** box(point3 a, point3 b, material* mat) {
    // 6 sides, so array of 6 quads
    object** sides = malloc(sizeof(object*) * 6);
    if (sides == NULL) {
        perror("Malloc error.");
        exit(1);
    }

    // opposite vertices with the min and max coordinates:
    point3 min = vec3_create(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
    point3 max = vec3_create(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));

    vec3 dx = vec3_create(max.x - min.x, 0, 0);
    vec3 dy = vec3_create(0, max.y - min.y, 0);
    vec3 dz = vec3_create(0, 0, max.z - min.z);

    // add all sides to the object array:
    sides[0] = quad_create(vec3_create(min.x, min.y, max.z), dx, dy, mat); // front
    sides[1] = quad_create(vec3_create(max.x, min.y, max.z), vec3_negative(dz), dy, mat); // right
    sides[2] = quad_create(vec3_create(max.x, min.y, min.z), vec3_negative(dx), dy, mat); // back
    sides[3] = quad_create(vec3_create(min.x, min.y, min.z), dz, dy, mat); // left
    sides[4] = quad_create(vec3_create(min.x, max.y, max.z), dx, vec3_negative(dz), mat); // top
    sides[5] = quad_create(vec3_create(min.x, min.y, max.z), dx, dz, mat); // bottom

    return sides;
}