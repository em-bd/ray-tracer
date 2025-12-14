#ifndef IMAGE_H
#define IMAGE_H

#include "rtw_stb_image.h"

/**
 * Image texture data structure:
 */
typedef struct {
    rtw_image* image;
} image;

image* image_create(const char*);

#endif