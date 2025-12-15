#include "triangle_mesh.h"

// PRIVATE METHODS

/**
 * Compute per-vertex normals for a triangle mesh:
 */
void compute_normals(triangle_mesh* t) {
    // initialize normals:
    t->normals = malloc(sizeof(vec3) * t->n_v);
    for (int i = 0; i < t->n_v; i++)
        t->normals[i] = vec3_create(0, 0, 0);

    // accumulate face normals:
    for (int i = 0; i < t->n_t; i++) {
        int i0 = t->indices[i*3];
        int i1 = t->indices[i*3 + 1];
        int i2 = t->indices[i*3 + 2];

        point3 v0 = t->vertices[i0];
        point3 v1 = t->vertices[i1];
        point3 v2 = t->vertices[i2];

        // face normal:
        vec3 e1 = vec3_sub(v1, v0);
        vec3 e2 = vec3_sub(v2, v0);
        vec3 face_normal = vec3_normalize(vec3_cross(e1, e2));

        // add to each vertex normal:
        t->normals[i0] = vec3_add(t->normals[i0], face_normal);
        t->normals[i1] = vec3_add(t->normals[i1], face_normal);
        t->normals[i2] = vec3_add(t->normals[i2], face_normal);
    }

    // normalize all vertex normals:
    for (int i = 0; i < t->n_v; i++)
        t->normals[i] = vec3_normalize(t->normals[i]);
}

/**
 * Determine if a ray hits a triangle mesh:
 */
bool hit_triangle_mesh(ray r, interval in, hit_record* rec, void* o) {
    triangle_mesh* tm = ((triangle_mesh*) ((object*) o)->data);
    return tm->bvh->hit(r, in, rec, tm->bvh);
}

/**
 * Bounding box for a triangle mesh:
 */
aabb mesh_bbox(triangle_mesh* t) {
    double min_x = infinity, min_y = infinity, min_z = infinity;
    double max_x = -infinity, max_y = -infinity, max_z = -infinity;

    for (int i = 0; i < t->n_v; i++) {
        point3 v = t->vertices[i];
        min_x = fmin(min_x, v.x);
        min_y = fmin(min_y, v.y);
        min_z = fmin(min_z, v.z);

        max_x = fmax(max_x, v.x);
        max_y = fmax(max_y, v.y);
        max_z = fmax(max_z, v.z);
    }

    point3 min = vec3_create(min_x - epsilon, min_y - epsilon, min_z - epsilon);
    point3 max = vec3_create(max_x + epsilon, max_y + epsilon, max_z + epsilon);

    return aabb_points(min, max);
}

// PUBLIC METHODS

/**
 * Create a triangle mesh object:
 */
object* triangle_mesh_create(point3* vertices, int n_v, int* indices, int n_t, material* mat) {
    triangle_mesh* tm = malloc(sizeof(triangle_mesh));
    if (tm == NULL) {
        perror("Malloc error.");
        exit(1);
    }

    tm->vertices = vertices;
    tm->n_v = n_v; // number of vertices
    tm->n_t = n_t; // number of triangles
    tm->indices = indices;
    tm->mat = mat;

    // get all triangles in the mesh:
    object** tris = malloc(sizeof(object*) * n_t);
    for (int i = 0; i < n_t; i++) {
        point3 a = vertices[3*i];
        point3 b = vertices[3*i + 1];
        point3 c = vertices[3*i + 2];
        tris[i] = triangle_create(a, b, c, mat);
    }

    // build the BVH for the triangles in the mesh:
    tm->bvh = build_bvh(tris, 0, n_t);
    compute_normals(tm);

    return object_create(triangle_mesh_obj, tm, mesh_bbox(tm), hit_triangle_mesh);
}