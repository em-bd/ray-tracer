#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "parser.h"
#include "texture-load.h"

#define frand() (rand() / (RAND_MAX + 1.0))

int main() {

    // Camera
    initialize();

    // Load objects
    object** objs = load(paths[0]);
    int i = 0;
    while (objs[i] != NULL) ++i;
    world = build_bvh(objs, 0, i);

    // Render scene
    render();

    printf("Done!\n");
    return 0;
}