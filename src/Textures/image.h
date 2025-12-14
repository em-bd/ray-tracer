#ifndef IMAGE_H
#define IMAGE_H

#include "rtw_stb_image.h"
#include "texture.h"

/**
 * Image texture data structure:
 */
typedef struct {
    rtw_image* image;
} image;

texture* image_create(const char*);

#endif