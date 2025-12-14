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

/**
 * Perform a trilinear interpolation:
 */
double perlin_interp(vec3 c[2][2][2], double u, double v, double w) {
    // hermitian smoothing:
    double uu = u*u*(3-2*u);
    double vv = v*v*(3-2*v);
    double ww = w*w*(3-2*w);
    double accum = 0.0;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                vec3 weight_v = vec3_create(u-i, v-j, w-k);
                accum += (i*uu + (1-i)*(1-uu))
                    * (j*vv + (1-j)*(1-vv))
                    * (k*ww + (1-k)*(1-ww))
                    * vec3_dot(c[i][j][k], weight_v);
            }
        }
    }

    return accum;
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
    per->randvec = malloc(sizeof(vec3) * per->point_count);
    interval in = interval_create(-1, 1);
    for (int i = 0; i < per->point_count; i++)
        per->randvec[i] = vec3_unit(vec3_random(in));

    per->perm_x = malloc(sizeof(int) * per->point_count);
    perlin_generate_perm(per, per->perm_x);
    per->perm_y = malloc(sizeof(int) * per->point_count);
    perlin_generate_perm(per, per->perm_y);
    per->perm_z = malloc(sizeof(int) * per->point_count);
    perlin_generate_perm(per, per->perm_z);
    return per;
}

double noise(perlin* per, point3* p) {
    double u = p->x - floor(p->x);
    double v = p->y - floor(p->y);
    double w = p->z - floor(p->z);

    int i = floor(p->x);
    int j = floor(p->y);
    int k = floor(p->z);

    vec3 c[2][2][2];

    for (int di = 0; di < 2; di++) {
        for (int dj = 0; dj < 2; dj++) {
            for (int dk = 0; dk < 2; dk++) {
                c[di][dj][dk] = per->randvec
                    [per->perm_x[(i + di) & 255] ^
                    per->perm_y[(j + dj) & 255] ^
                    per->perm_z[(k + dk) & 255]];
            }
        }
    }

    return perlin_interp(c, u, v, w);
}

double turb(perlin* per, point3* p, int depth) {
    double accum = 0.0;
    point3 temp = *p;
    double weight = 1.0;

    for (int i = 0; i < depth; i++) {
        accum += weight * noise(per, &temp);
        weight *= 0.5;
        temp = vec3_scalar(temp, 2);
    }

    return fabs(accum);
}