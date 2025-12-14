#include "quad.h"

/**
 * Create a quadrilateral object:
 */
quad* quad_create(point3 Q, vec3 u, vec3 v, material* mat) {
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

    return q;
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