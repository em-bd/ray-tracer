#include "parser.h"

#define path "../Objects/objects.csv"

FILE* f = NULL;

/**
 * Loading a sphere from a CSV file:
 */
void loadSphere(int i) {
    double x = atof(strtok(NULL, ","));
    double y = atof(strtok(NULL, ","));
    double z = atof(strtok(NULL, ","));
    double r = atof(strtok(NULL, ","));

    // allocate:
    sphere* s = malloc(sizeof(sphere));
    if (s == NULL) {
        perror("malloc error");
        exit(1);
    }

    s->center = vec3_create(x, y, z);
    s->radius = r;
    
    object* o = malloc(sizeof(object));
    o->data = s;
    o->type = sphere_obj;
    objects[i] = o;
}

/**
 * Loading a triangle from a CSV file:
 */
void loadTriangle(int i) {
    double x0 = atof(strtok(NULL, ","));
    double y0 = atof(strtok(NULL, ","));
    double z0 = atof(strtok(NULL, ","));
    double x1 = atof(strtok(NULL, ","));
    double y1 = atof(strtok(NULL, ","));
    double z1 = atof(strtok(NULL, ","));
    double x2 = atof(strtok(NULL, ","));
    double y2 = atof(strtok(NULL, ","));
    double z2 = atof(strtok(NULL, ","));

    triangle* t = malloc(sizeof(triangle));
    t->a = vec3_create(x0, y0, z0);
    t->b = vec3_create(x1, y1, z1);
    t->c = vec3_create(x2, y2, z2);

    object* o = malloc(sizeof(object));
    o->data = t;
    o->type = triangle_obj;
    objects[i] = o;
}

/**
 * Load all objects into the objects array from a CSV file:
 */
void load() {
    // open file and make sure it exists:
    f = fopen(path, "r");
    if (!f) {
        perror("File open error");
        exit(1);
    }

    char line[256];
    int i, capacity;
    i = 0;
    capacity = 10;
    objects = malloc(sizeof(object*) * capacity);
    if (objects == NULL) {
        perror("malloc error");
        exit(1);
    }

    // read each line of the file:
    while (fgets(line, sizeof(line), f)) {
        // exceeding current size of object array, double capacity:
        if (i >= capacity) {
            capacity *= 2;
            objects = realloc(objects, sizeof(object*) * capacity);
        }

        char* type = strtok(line, ",");
        if (!type)
            continue;

        if (strcmp(type, "sphere") == 0)
            loadSphere(i++);
        if (strcmp(type, "triangle") == 0)
            loadTriangle(i++);
    }
    // close file:
    fclose(f);

    // null terminate the objects array:
    objects[i] = NULL;
}