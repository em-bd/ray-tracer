#include "camera.h"

camera* c = NULL;
object** objects = NULL;

vec3 sample_square() {
    return vec3_create(rand_double() - 0.5, rand_double() - 0.5, 0);
}

/**
 * Returns a random point in the camera defocus disk:
 */
point3 defocus_disk_sample() {
    vec3 p = random_in_unit_disk();
    return vec3_add(c->center, vec3_add(vec3_scalar(c->defocus_disk_u, p.x), vec3_scalar(c->defocus_disk_v, p.y)));
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

    point3 orig = (c->defocus_angle <= 0) ? c->center : defocus_disk_sample();

    return ray_create(orig, vec3_sub(pixel_sample, orig));
}

/**
 * Initialize the camera:
 */
void initialize() {
    c = malloc(sizeof(camera));
    c->pixel_samples_scale = 1.0 / ((double) SAMPLES_PER_PIXEL);
    c->max_depth = 50;

    c->aspect_ratio = 16.0 / 9.0;
    c->image_width = 400;
    c->image_height = (int) (c->image_width / c->aspect_ratio);
    c->image_height = (c->image_height < 1) ? 1 : c->image_height;

    // configurable camera variables:
    c->center = c->lookfrom = vec3_create(-2, 2, 1);       // point looking from and camera center
    c->lookat = vec3_create(0, 0, -1.0);                    // point looking at
    c->vfov = 90;                                           // field of view
    c->vup = vec3_create(0, 1.0, 0);                        // relative "up" direction

    // viewport dimensions:
    c->defocus_angle = 3;
    c->focus_dist = vec3_length(vec3_sub(c->lookfrom, c->lookat));
    double theta = degrees_to_radians(c->vfov);
    double h = tan(theta/2.0);
    double viewport_height = 2.0 * h * c->focus_dist;
    double viewport_width = viewport_height * ((double) c->image_width / (double) c->image_height);

    // unit basics vectors:
    c->w = vec3_unit(vec3_sub(c->lookfrom, c->lookat));
    c->u = vec3_unit(vec3_cross(c->vup, c->w));
    c->v = vec3_cross(c->w, c->u);

    // viewport edge vectors:
    vec3 viewport_u = vec3_scalar(c->u, viewport_width);
    vec3 viewport_v = vec3_scalar(c->v, -viewport_height);

    // horizontal and vertical delta vectors from pixel to pixel:
    c->pixel_delta_u = vec3_scalar(viewport_u, (1.0 / c->image_width));
    c->pixel_delta_v = vec3_scalar(viewport_v, (1.0 / c->image_height));

    // find location of upper left pixel of the viewport:
    vec3 r1 = vec3_sub(c->center, vec3_scalar(c->w, c->focus_dist));

    vec3 viewport_upper_left = vec3_sub(vec3_sub(r1, vec3_scalar(viewport_u, 0.5)),
                                    vec3_scalar(viewport_v, 0.5));
    
    vec3 r3 = vec3_scalar(vec3_add(c->pixel_delta_u, c->pixel_delta_v), 0.5);
    c->pixel00_loc = vec3_add(viewport_upper_left, r3);

    // defocus disk basics vectors:
    double defocus_radius = c->focus_dist * tan(degrees_to_radians(c->defocus_angle / 2.0));
    c->defocus_disk_u = vec3_scalar(c->u, defocus_radius);
    c->defocus_disk_v = vec3_scalar(c->v, defocus_radius);
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
                // pixel color with sampling:
                color pixel_color = vec3_create(0, 0, 0);
                for (int s = 0; s < SAMPLES_PER_PIXEL; s++) {
                    ray r = get(i, j);
                    pixel_color = vec3_add(pixel_color, ray_color(r, c->max_depth, objects));
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
color ray_color(ray r, int depth, object** objs) {
    // if we've exceeded the ray bounce limit, no more light is gathered:
    if (depth <= 0)
        return vec3_create(0, 0, 0);
    // check if the ray intersects anything:
    hit_record rec;
    if (hit(r, interval_create(0.001, infinity), &rec, objs)) {
        // vec3 direction = vec3_add(rec.normal, random_unit_vector());
        // return vec3_scalar(ray_color(ray_create(rec.p, direction), --depth, objects), 0.1);
        ray scattered;
        color attenuation;
        if (scatter_func[rec.mat->type](r, rec, &attenuation, &scattered, rec.mat))
            return vec3_mul(ray_color(scattered, --depth, objects), attenuation);
        return vec3_create(0, 0, 0);
    }

    vec3 unit_dir = vec3_unit(r.dir);
    double a = 0.5 * (unit_dir.y + 1.0);

    color white = vec3_create(1.0, 1.0, 1.0);
    color blue = vec3_create(0.5, 0.7, 1.0);

    return vec3_add(vec3_scalar(white, (1.0 - a)), vec3_scalar(blue, a));
}