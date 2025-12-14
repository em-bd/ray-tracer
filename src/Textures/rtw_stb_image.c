#define STB_IMAGE_IMPLEMENTATION

#include "rtw_stb_image.h"
#include "../Assets/stb_image.h"

// PRIVATE FUNCTIONS

/**
 * Return value clamped to the range [low, high)
 */
int img_clamp(int x, int low, int high) {
    if (x < low) return low;
    if (x >= high) return high - 1;
    return x;
}

/**
 * Convert from float to byte:
 */
unsigned char float_to_byte(float value) {
    if (value <= 0.0)
        return 0;
    if (1.0 <= value)
        return 255;
    return (unsigned char) (255.999 * value);
}

/**
 * Convert linear floating point pixel data to bytes
 * Store resulting byte data
 */
void convert_to_bytes(rtw_image* img) {
    int total_bytes = img->image_width * img->image_height * img->bytes_per_pixel;
    img->bdata = malloc(sizeof(unsigned char) * total_bytes);

    // Iterate through all pixel components
    unsigned char* bptr = img->bdata;
    float* fptr = img->fdata;
    for (int i = 0; i < total_bytes; i++, fptr++, bptr++)
        *bptr = float_to_byte(*fptr);
}

// PUBLIC FUNCTIONS

rtw_image* rtw_image_create(const char* path) {
    rtw_image* img = malloc(sizeof(rtw_image));
    if (img == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    img->bytes_per_pixel = 3;
    img->bytes_per_scanline = 0;
    img->image_height = 0;
    img->image_width = 0;

    char filepath[256];
    sprintf(filepath, "Assets/%s", path);
    // image file exists, successfully load:
    if (load(filepath, img))
        return img;
    else {
        perror("Load error, no such image");
        exit(1);
    }

    // no image, return null:
    free(img);
    return NULL;
}

/**
 * Load the image and return if the image load was successful:
 */
bool load(const char* path, rtw_image* img) {
    int n = img->bytes_per_pixel;
    img->fdata = stbi_loadf(path, &img->image_width, &img->image_height, &n, img->bytes_per_pixel);
    if (img->fdata == NULL)
        return false;

    img->bytes_per_scanline = img->image_width * img->bytes_per_pixel;
    convert_to_bytes(img);
    return true;
}

/**
 * Return the address of the three RGB bytes of the pixel at x, y
 * No image data, return magenta
 */
const unsigned char* pixel_data(rtw_image* img, int x, int y) {
    static const unsigned char magenta[] = {255, 0, 255};
    if (img->bdata == NULL)
        return magenta;

    x = img_clamp(x, 0, img->image_width);
    y = img_clamp(y, 0, img->image_height);

    return img->bdata + y*img->bytes_per_scanline + x*img->bytes_per_pixel;
}

/**
 * Free image:
 */
void free_img(rtw_image* img) {
    if (img == NULL)
        return;

    if (img->fdata != NULL)
        stbi_image_free(img->fdata);
    if (img->bdata != NULL)
        free(img->bdata);
}