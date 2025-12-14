#include "image.h"

/**
 * Create an image texture from a file path:
 */
image* image_create(const char* path) {
    image* i = malloc(sizeof(image));
    if (i == NULL) {
        perror("Malloc error.");
        exit(1);
    }

    i->image = rtw_image_create(path);
    
    return i;
}