#include "triangle.h"

// PRIVATE FUNCTIONS

/**
 * Sets the normal to face outward from the surface:
 */
void tri_set_face_normal(ray r, vec3 outward_normal, hit_record* rec) {
    rec->front_face = vec3_dot(r.dir, outward_normal) < 0;
    rec->normal = rec->front_face ? outward_normal : vec3_negative(outward_normal);
}

/**
 * Determine if the ray hits the triangle:
 */
bool hit_triangle(ray r, interval i, hit_record* rec, void* o) {
    triangle* tri = ((triangle*) ((object*) o)->data);

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
    tri_set_face_normal(r, outward_normal, rec);
    rec->mat = tri->mat;
    rec->u = x;
    rec->v = y;

    return true;
}

/**
 * Triangle bounding box function
 */
aabb triangle_box(triangle* t) {
    double x_min = fmin(t->a.x, fmin(t->b.x, t->c.x));
    double y_min = fmin(t->a.y, fmin(t->b.y, t->c.y));
    double z_min = fmin(t->a.z, fmin(t->b.z, t->c.z));

    double x_max = fmax(t->a.x, fmax(t->b.x, t->c.x));
    double y_max = fmax(t->a.y, fmax(t->b.y, t->c.y));
    double z_max = fmax(t->a.z, fmax(t->b.z, t->c.z));

    point3 min = vec3_create(x_min - epsilon, y_min - epsilon, z_min - epsilon);
    point3 max = vec3_create(x_max + epsilon, y_max + epsilon, z_max + epsilon);

    return aabb_points(min, max);
}

// PUBLIC FUNCTIONS

/**
 * Create a triangle:
 */
object* triangle_create(point3 a, point3 b, point3 c, material* mat) {
    triangle* tri = malloc(sizeof(triangle));
    if (tri == NULL) {
        perror("Malloc error.");
        exit(1);
    }

    // consistent winding:
    vec3 e1 = vec3_sub(b, a);
    vec3 e2 = vec3_sub(c, a);
    vec3 n = vec3_cross(e1, e2);

    if (n.z < 0) {
        point3 tmp = b;
        b = c;
        c = tmp;
    }

    tri->a = a;
    tri->b = b;
    tri->c = c;
    tri->mat = mat;

    tri->bbox = triangle_box(tri);

    return object_create(triangle_obj, tri, tri->bbox, hit_triangle);
}