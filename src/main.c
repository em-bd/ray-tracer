#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "camera.h"
#include "texture-load.h"

#define frand() (rand() / (RAND_MAX + 1.0))

// ASPECT RATIO

#define     SAMPLES_PER_PIXEL   100

int main() {
    FILE *f;

    // Camera
    camera *c = malloc(sizeof(camera));
    initialize(c);

    // list of objects
    sphere** spheres = malloc(sizeof(sphere*) * 3);
    spheres[0] = malloc(sizeof(sphere));
    spheres[0]->center = vec3_create(0, 0, -1);
    spheres[0]->radius = 0.5;
    spheres[1] = malloc(sizeof(sphere));
    spheres[1]->center = vec3_create(0, -100.5, -1);
    spheres[1]->radius = 100;
    spheres[2] = NULL;

    // Render
    f = fopen("image.ppm", "wb");
    fprintf(f, "P3\n%d\n%d\n255\n", c->image_width, c->image_height);


        for (int j = 0; j < c->image_height; j++) {
            for (int i = 0; i < c->image_width; i++) {
                // pixel center logic:
                vec3 r1 = vec3_add(vec3_scalar(c->pixel_delta_u, (double) i), 
                                    vec3_scalar(c->pixel_delta_v, (double) j));
                point3 pixel_center = vec3_add(c->pixel00_loc, r1);

                // pixel color logic:
                ray r;
                r.dir = vec3_sub(pixel_center, c->center);
                r.orig = c->center;
                color pixel_color = ray_color(r, spheres);

                // write to file:
                char * restrict line = malloc(sizeof(char) * 20);
                write_color(line, pixel_color);
                fprintf(f, "%s", line);
                free(line);
            }
        }
    for (int i = 0; spheres[i] != NULL; i++) {
        free(spheres[i]);
    }
    free(spheres);
    fclose(f);

    return 0;
}