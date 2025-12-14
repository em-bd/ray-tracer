#include "triangle.h"

/**
 * Create a triangle:
 */
triangle* triangle_create(point3 a, point3 b, point3 c, material* mat) {
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

    return tri;
}