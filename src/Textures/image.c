#include "image.h"

/**
 * Get the value for an image texture:
 */
color image_value(void* t, double* u, double* v, point3* p) {
    image* img = ((image*) ((texture*) t)->data);
    if (img == NULL || img->image == NULL || img->image->image_height <= 0)
        return vec3_create(0, 1, 1);

    interval i0 = interval_create(0, 1);
    *u = clamp(i0, *u);
    *v = clamp(i0, *v);

    int i = (int) (*u * img->image->image_width);
    i = (i >= img->image->image_width) ? img->image->image_width - 1 : i;
    int j = (int) ((1.0 - *v) * img->image->image_height);
    j = (j >= img->image->image_height) ? img->image->image_height - 1 : j;
    const unsigned char* pixel = pixel_data(img->image, i, j);

    double color_scale = 1.0/ 255.0;
    return vec3_create(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
}

/**
 * Create an image texture from a file path:
 */
texture* image_create(const char* path) {
    image* i = malloc(sizeof(image));
    if (i == NULL) {
        perror("Malloc error.");
        exit(1);
    }

    i->image = rtw_image_create(path);
    if (!i) {
        free(i);
        return NULL;
    }
    return texture_create(image_tex, i, image_value);
}