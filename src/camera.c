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
color ray_color(ray ray, object** objs) {
    // check if the ray intersects a sphere:
    for (int i = 0; objs[i] != NULL; i++) {
        hit_record rec;
        interval in = interval_create(0, infinity);
        if (hit_func[objs[i]->type](ray, in, &rec, objs[i])) {
            color c = vec3_create(1.0, 1.0, 1.0);
            return vec3_scalar(vec3_add(rec.normal, c), 0.5);
        }
    }

    vec3 unit_dir = vec3_unit(ray.dir);
    double a = 0.5 * (unit_dir.y + 1.0);

    color white = vec3_create(1.0, 1.0, 1.0);
    color blue = vec3_create(0.5, 0.7, 1.0);

    return vec3_add(vec3_scalar(white, (1.0 - a)), vec3_scalar(blue, a));
}