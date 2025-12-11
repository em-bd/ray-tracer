#include "color.h"

/**
 * Linear to Gamma transform:
 */
double linear_to_gamma(double comp) {
    if (comp > 0)
        return sqrt(comp);

    return 0.0;
}

/**
 * Write a single pixel's color to the console output:
 */
const char* write_color(char* restrict line, color pixel_color) {
    // apply linear to gamma transform:
    double r = linear_to_gamma(pixel_color.x);
    double g = linear_to_gamma(pixel_color.y);
    double b = linear_to_gamma(pixel_color.z);

    interval i = interval_create(0.000, 0.999);

    int rbyte = (int) (256 * clamp(i, r));
    int gbyte = (int) (256 * clamp(i, g));
    int bbyte = (int) (256 * clamp(i, b));

    sprintf(line, "%d %d %d\n", rbyte, gbyte, bbyte);
    return line;
}