#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "camera.h"
#include "scenes.h"

#define frand() (rand() / (RAND_MAX + 1.0))

int main() {

    // Camera
    initialize();

    // Load objects
    printf("Generating objects\n");
    // bouncing_spheres();
    // checkered_spheres();
    world_sphere();
    printf("Done generating.\n");
    int i = 0;
    while (objects[i] != NULL) ++i;
    world = build_bvh(objects, 0, i);
    printf("BVH done.\n");

    // Render scene
    render();

    printf("Done!\n");
    return 0;
}