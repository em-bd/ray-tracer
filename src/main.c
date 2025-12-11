#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "parser.h"
#include "texture-load.h"

#define frand() (rand() / (RAND_MAX + 1.0))

// ASPECT RATIO

#define     SAMPLES_PER_PIXEL   100

int main() {

    // Camera
    initialize();

    // List of objects
    // objects = malloc(sizeof(object*) * 3);
    // objects[0] = malloc(sizeof(object));
    // objects[0]->data = malloc(sizeof(sphere));
    // ((sphere *) (objects[0])->data)->center = vec3_create(0, 0, -1.0);
    // ((sphere *) (objects[0])->data)->radius = 0.5;
    // objects[0]->type = sphere_obj;
    // objects[1] = malloc(sizeof(object));
    // objects[1]->data = malloc(sizeof(sphere));
    // ((sphere *) (objects[1])->data)->center = vec3_create(0, -100.5, -1);
    // ((sphere *) (objects[1])->data)->radius = 100;
    // objects[1]->type = sphere_obj;
    // objects[2] = NULL;
    load();

    // Render scene
    render();

    if (c != NULL)
        free(c);

    return 0;
}