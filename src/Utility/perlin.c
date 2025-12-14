#include "perlin.h"

// PRIVATE FUNCTIONS

void permute(int* p, int n) {
    for (int i = n - 1; i > 0; i--) {
        int target = random_int(0, i);
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

void perlin_generate_perm(perlin* per, int* p) {
    for (int i = 0; i < per->point_count; i++)
        p[i] = i;

    permute(p, per->point_count);
}

// PUBLIC FUNCTIONS

/**
 * Create a Perlin object:
 */
perlin* perlin_create() {
    perlin* per = malloc(sizeof(perlin));
    if (per == NULL) {
        perror("Malloc error");
        exit(1);
    }
    per->point_count = 256;
    for (int i = 0; i < per->point_count; i++)
        per->rand_float[i] = rand_double();

    per->perm_x = malloc(sizeof(int) * per->point_count);
    perlin_generate_perm(per, per->perm_x);
    per->perm_y = malloc(sizeof(int) * per->point_count);
    perlin_generate_perm(per, per->perm_y);
    per->perm_z = malloc(sizeof(int) * per->point_count);
    perlin_generate_perm(per, per->perm_z);
    return per;
}

double noise(perlin* per, point3* p) {
    int i = (int) (4 * p->x) & 255;
    int j = (int) (4 * p->y) & 255;
    int k = (int) (4 * p->z) & 255;

    return per->rand_float[per->perm_x[i] ^ per->perm_y[j] ^ per->perm_z[k]];
}