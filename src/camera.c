#include "camera.h"

camera* c = NULL;
object** objects = NULL;

vec3 sample_square() {
    return vec3_create(rand_double() - 0.5, rand_double() - 0.5, 0);
}

/**
 * Construct a camera ray originating from the origin and directed at a
 * randomly sampled point around location i,j:
 */
ray get(int i, int j) {
    vec3 offset = sample_square();
    vec3 pixel_sample = vec3_add(
        c->pixel00_loc,
        vec3_add(
            vec3_scalar(c->pixel_delta_u, ((double) i + offset.x)),
            vec3_scalar(c->pixel_delta_v, ((double) j + offset.y))
        )
    );
    
    ray r;
    r.orig = c->center;
    r.dir = vec3_sub(pixel_sample, r.orig);

    return r;
}

/**
 * Initialize the camera:
 */
void initialize() {
    c = malloc(sizeof(camera));
    c->pixel_samples_scale = 1.0 / ((double) SAMPLES_PER_PIXEL);

    c->aspect_ratio = 16.0 / 9.0;
    c->image_width = 400;
    c->image_height = (int) (c->image_width / c->aspect_ratio);
    c->image_height = (c->image_height < 1) ? 1 : c->image_height;

    c->center = vec3_create(0, 0, 0);

    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * ((double) c->image_width / (double) c->image_height);

    // calculate the vectors across the horizontal and down the vertical viewport edges:
    vec3 viewport_u = vec3_create(viewport_width, 0, 0);
    vec3 viewport_v = vec3_create(0, -viewport_height, 0);

    // calculate horizontal and vertical delta vectors from pixel to pixel:
    c->pixel_delta_u = vec3_scalar(viewport_u, (double) (1.0 / c->image_width));
    c->pixel_delta_v = vec3_scalar(viewport_v, (double) (1.0 / c->image_height));

    // find location of upper left pixel of the viewport:
    vec3 r1 = vec3_sub(c->center, vec3_create(0, 0, focal_length));

    vec3 viewport_upper_left = vec3_sub(vec3_sub(r1, vec3_scalar(viewport_u, 0.5)),
                                    vec3_scalar(viewport_v, 0.5));
    
    vec3 r3 = vec3_scalar(vec3_add(c->pixel_delta_u, c->pixel_delta_v), 0.5);
    c->pixel00_loc = vec3_add(viewport_upper_left, r3);
}

/**
 * Render the scene with the provided list of objects:
 */
void render() {
    FILE *f;
    f = fopen("image.ppm", "wb");
    fprintf(f, "P3\n%d\n%d\n255\n", c->image_width, c->image_height);

        for (int j = 0; j < c->image_height; j++) {
            for (int i = 0; i < c->image_width; i++) {
                // // pixel center logic:
                // vec3 r1 = vec3_add(vec3_scalar(c->pixel_delta_u, (double) i), 
                //                     vec3_scalar(c->pixel_delta_v, (double) j));
                // point3 pixel_center = vec3_add(c->pixel00_loc, r1);

                // // pixel color logic:
                // ray r;
                // r.dir = vec3_sub(pixel_center, c->center);
                // r.orig = c->center;
                // color pixel_color = ray_color(r, objects);
                color pixel_color = vec3_create(0, 0, 0);
                for (int s = 0; s < SAMPLES_PER_PIXEL; s++) {
                    ray r = get(i, j);
                    pixel_color = vec3_add(pixel_color, ray_color(r, objects));
                }

                // write to file:
                char * restrict line = malloc(sizeof(char) * 20);
                write_color(line, vec3_scalar(pixel_color, c->pixel_samples_scale));
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
}

/**
 * Determine the color vector of the ray:
 * (currently makes the ray a blue-white gradient)
 */
color ray_color(ray ray, object** objs) {
    // check if the ray intersects anything:
    hit_record rec;
    if (hit(ray, interval_create(0, infinity), &rec, objs)) {
        color c = vec3_create(1.0, 1.0, 1.0);
        return vec3_scalar(vec3_add(rec.normal, c), 0.5);
    }

    vec3 unit_dir = vec3_unit(ray.dir);
    double a = 0.5 * (unit_dir.y + 1.0);

    color white = vec3_create(1.0, 1.0, 1.0);
    color blue = vec3_create(0.5, 0.7, 1.0);

    return vec3_add(vec3_scalar(white, (1.0 - a)), vec3_scalar(blue, a));
}