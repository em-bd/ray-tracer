#include "parser.h"

object** objects = NULL;

/**
 * Array of all test scenes:
 */
char* paths[5] = {
    "../Assets/scene1.csv",
    "../Assets/scene2.csv",
    "../Assets/scene3.csv",
    "../Assets/scene4.csv",
    "../Assets/scene5.csv",
};

FILE* f = NULL;

/**
 * Load a metal material onto an object:
 */
material* loadMetal() {
    material* mat = malloc(sizeof(material));
    if (mat == NULL) {
        perror("Malloc error");
        exit(1);
    }
    metal* m = malloc(sizeof(metal));
    if (m == NULL) {
        perror("Malloc error");
        exit(1);
    }

    double x = atof(strtok(NULL, ","));
    double y = atof(strtok(NULL, ","));
    double z = atof(strtok(NULL, ","));
    double fuzz = atof(strtok(NULL, ","));

    *m = metal_create(x, y, z, fuzz);
    mat->data = m;
    mat->type = metal_type;

    return mat;
}

/**
 * Load a lambertian material onto an object:
 */
material* loadLambertian() {
    material* mat = malloc(sizeof(material));
    if (mat == NULL) {
        perror("Malloc error");
        exit(1);
    }
    lambertian* l = malloc(sizeof(lambertian));
    if (l == NULL) {
        perror("Malloc error");
        exit(1);
    }

    double x = atof(strtok(NULL, ","));
    double y = atof(strtok(NULL, ","));
    double z = atof(strtok(NULL, ","));

    *l = lambertian_create(x, y, z);
    mat->data = l;
    mat->type = lambertian_type;

    return mat;
}

/**
 * Load a dielectric material:
 */
material* loadDielectric() {
    material* mat = malloc(sizeof(material));
    if (mat == NULL) {
        perror("Malloc error");
        exit(1);
    }
    dielectric* d = malloc(sizeof(dielectric));
    if (d == NULL) {
        perror("Malloc error");
        exit(1);
    }

    double refraction_index = atof(strtok(NULL, ","));

    *d = dielectric_create(refraction_index);
    mat->data = d;
    mat->type = dielectric_type;

    return mat;
}

/**
 * Loading a sphere from a CSV file:
 */
void loadSphere(int i) {
    double x = atof(strtok(NULL, ","));
    double y = atof(strtok(NULL, ","));
    double z = atof(strtok(NULL, ","));
    double r = atof(strtok(NULL, ","));

    char* mat = strtok(NULL, ",");
    if (strcmp(mat, "metal") == 0)
        objects[i] = sphere_create(vec3_create(x, y, z), r, loadMetal());
    else if (strcmp(mat, "lambertian") == 0)
        objects[i] = sphere_create(vec3_create(x, y, z), r, loadLambertian());
    else if (strcmp(mat, "dielectric") == 0)
        objects[i] = sphere_create(vec3_create(x, y, z), r, loadDielectric());
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
    if (t == NULL) {
        perror("Malloc error");
        exit(1);
    }
    t->a = vec3_create(x0, y0, z0);
    t->b = vec3_create(x1, y1, z1);
    t->c = vec3_create(x2, y2, z2);

    object* o = malloc(sizeof(object));
    if (o == NULL) {
        perror("Malloc error");
        exit(1);
    }
    o->data = t;
    o->type = triangle_obj;
    objects[i] = o;
}

/**
 * Load all objects into the objects array from a CSV file:
 */
object** load(const char* path) {
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
            if (objects == NULL) {
                perror("Realloc error");
                exit(1);
            }
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
    return objects;
}