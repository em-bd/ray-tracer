#ifndef RTW_STB_IMAGE_H
#define RTW_STB_IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int bytes_per_pixel;
    float* fdata;
    unsigned char* bdata;
    int image_width;
    int image_height;
    int bytes_per_scanline;
} rtw_image;

rtw_image* rtw_image_create(const char*);

bool load(const char*, rtw_image*);

int get_image_width(rtw_image);
int get_image_height(rtw_image);

const unsigned char* pixel_data(rtw_image*, int, int);

void free_img(rtw_image*);

#endif