#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "../Utility/utils.h"
#include "../Materials/material.h"
#include "object.h"
#include "bvh_node.h"
#include "triangle.h"

typedef struct {
    int n_t, n_v;
    int* indices;
    point3* vertices;
    vec3* normals;
    material* mat;
    object* bvh;
} triangle_mesh;

object* triangle_mesh_create(point3*, int, int*, int, material*);

#endif