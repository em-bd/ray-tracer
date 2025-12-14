#include "matrix.h"

point3 transform(point3 v, vec3 rotation[3], double scale, vec3 translation) {
    v = vec3_scalar(v, scale);

    //  TODO: set up transforms
    point3 rotated = vec3_create(0,0,0);
    
    return rotated;
}