#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "object.h"

typedef struct {
    object* object;
    vec3 offset;
} translate;

object* translate_create(object*, vec3);

#endif