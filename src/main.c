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
    object** objects = malloc(sizeof(object*) * 3);
    objects[0] = malloc(sizeof(object));
    objects[0]->data = malloc(sizeof(sphere));
    ((sphere *) (objects[0])->data)->center = vec3_create(0, 0, -1.0);
    ((sphere *) (objects[0])->data)->radius = 0.5;
    objects[0]->type = sphere_obj;
    objects[1] = malloc(sizeof(object));
    objects[1]->data = malloc(sizeof(sphere));
    ((sphere *) (objects[1])->data)->center = vec3_create(0, -100.5, -1);
    ((sphere *) (objects[1])->data)->radius = 100;
    objects[1]->type = sphere_obj;
    objects[2] = NULL;

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
                color pixel_color = ray_color(r, objects);

                // write to file:
                char * restrict line = malloc(sizeof(char) * 20);
                write_color(line, pixel_color);
                fprintf(f, "%s", line);
                free(line);
            }
        }
    for (int i = 0; objects[i] != NULL; i++) {
        free(objects[i]->data);
        free(objects[i]);
    }
    free(objects);
    fclose(f);

    return 0;
}