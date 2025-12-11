#include "color.h"

/**
 * Write a single pixel's color to the console output:
 */
const char* write_color(char* restrict line, color pixel_color) {
    // gamma correction
    double r = sqrt(pixel_color.x);
    double g = sqrt(pixel_color.y);
    double b = sqrt(pixel_color.z);

    interval i = interval_create(0.000, 0.999);

    int rbyte = (int) (256 * clamp(i, r));
    int gbyte = (int) (256 * clamp(i, g));
    int bbyte = (int) (256 * clamp(i, b));

    sprintf(line, "%d %d %d\n", rbyte, gbyte, bbyte);
    return line;
}