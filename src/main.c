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
    load(paths[0]);

    // Render scene
    render();

    if (c != NULL)
        free(c);

    return 0;
}