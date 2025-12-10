#include "camera.h"

/**
 * Initialize the camera:
 */
void initialize(camera *c) {
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
 * Determine the color vector of the ray:
 * (currently makes the ray a blue-white gradient)
 */
color ray_color(ray ray, sphere s) {
    double t = hit_sphere(ray, s);
    if (t > 0.0) {
        vec3 N = vec3_unit(vec3_sub(at(ray, t), s.center));
        vec3 v = vec3_create(N.x + 1, N.y + 1, N.z + 1);
        return vec3_scalar(v, 0.5);
    }

    // // check if the ray intersects a sphere:
    // for (int i = 0; spheres[i] != NULL; i++) {
    //     hit_record *rec = malloc(sizeof(hit_record));
    //     if (hitSphere(ray, 0, infinity, rec, *spheres[i])) {
    //         color hit;
    //         color c;
    //         c.x = c.y = c.z = 1.0;
    //         vec3_add(&hit, *rec->normal, c);
    //         vec3_scalar(&hit, hit, 0.5);
    //         return hit;
    //     }
    // }

    vec3 unit_dir = vec3_unit(ray.dir);
    double a = 0.5 * (unit_dir.y + 1.0);

    color white = vec3_create(1.0, 1.0, 1.0);
    color blue = vec3_create(0.5, 0.7, 1.0);

    return vec3_add(vec3_scalar(white, (1.0 - a)), vec3_scalar(blue, a));
}