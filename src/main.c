#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "parser.h"

#define frand() (rand() / (RAND_MAX + 1.0))

object** objects = NULL;

// random scene generation:
void random_generate() {
    int capacity = 10, i = 0;
    objects = malloc(sizeof(object*) * capacity);
    if (objects == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    texture* tex = checkered_create_from_solids(0.32, vec3_create(0.2, 0.3, .1), vec3_create(.9, .9, .9));
    material* ground = lambertian_create(tex);
    objects[i++] = sphere_create(vec3_create(0, -1000, 0), 1000, ground);

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            // reallocate objects
            if (i >= capacity) {
                capacity *= 2;
                objects = realloc(objects, sizeof(object*) * capacity);
                if (objects == NULL) {
                    perror("Realloc error.");
                    exit(1);
                }
            }
            double choose_mat = rand_double();
            point3 center = vec3_create(a + 0.9*rand_double(), 0.2, b + 0.9*rand_double());

            if (vec3_length(vec3_sub(center, vec3_create(4, 0.2, 0))) > 0.9) {
                material* mat = malloc(sizeof(material));
                if (mat == NULL) {
                    perror("Malloc error.");
                    exit(1);
                }
                // diffuse
                if (choose_mat < 0.8) {
                    color albedo = vec3_mul(vec3_rand(), vec3_rand());
                    texture* t = solid_create(albedo.x, albedo.y, albedo.z);
                    mat = lambertian_create(t);
                    point3 center2 = vec3_add(center, vec3_create(0, random_double(interval_create(0, 0.5)), 0));
                    objects[i++] = moving_sphere_create(center, center2, 0.2, mat);
                }
                // metal
                else if (choose_mat < 0.95) {
                    color albedo = vec3_random(interval_create(0.5, 1));
                    double fuzz = random_double(interval_create(0, 0.5));
                    mat = metal_create(albedo, fuzz);
                    objects[i++] = sphere_create(center, 0.2, mat);
                }
                // dielectric
                else {
                    mat = dielectric_create(1.5);
                    objects[i++] = sphere_create(center, 0.2, mat);
                }
            }
        }
    }
    objects[i] = NULL;
}

int main() {

    // Camera
    initialize();

    // Load objects
    printf("Generating objects\n");
    // objects = load(paths[0]);
    random_generate();
    printf("Done generating.\n");
    int i = 0;
    while (objects[i] != NULL) ++i;
    world = build_bvh(objects, 0, i);

    // Render scene
    render();

    printf("Done!\n");
    return 0;
}