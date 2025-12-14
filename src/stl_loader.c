#include "stl_loader.h"

object* load_binary_stl(const char* filename, material* mat) {
    
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "Assets/%s", filename);
    FILE* f = fopen(filepath, "rb");
    if (!f) {
        perror("Error opening STL file");
        exit(1);
    }

    // Skip 80-byte header
    char header[80];
    fread(header, 1, 80, f);

    // Read number of triangles
    uint32_t n_triangles;
    fread(&n_triangles, sizeof(uint32_t), 1, f);

    // Allocate arrays
    int n_vertices = n_triangles * 3;
    point3* vertices = malloc(sizeof(point3) * n_vertices);
    int* indices = malloc(sizeof(int) * n_triangles * 3);

    if (!vertices || !indices) {
        perror("Malloc error");
        fclose(f);
        exit(1);
    }

    // Read each triangle
    for (uint32_t i = 0; i < n_triangles; i++) {
        float normal[3];
        float v[9];
        uint16_t attr;

        fread(normal, sizeof(float), 3, f);   // face normal (often ignored)
        fread(v, sizeof(float), 9, f);        // 3 vertices
        fread(&attr, sizeof(uint16_t), 1, f); // attribute byte count

        // Store vertices
        for (int j = 0; j < 3; j++) {
            vertices[i*3 + j] = vec3_create(v[j*3], v[j*3+1], v[j*3+2]);
            indices[i*3 + j] = i*3 + j;
        }
    }

    fclose(f);

    // Build triangle mesh object
    return triangle_mesh_create(vertices, n_vertices, indices, n_triangles, mat);
}