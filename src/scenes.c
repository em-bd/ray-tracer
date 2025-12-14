#include "scenes.h"

object** objects = NULL;

/**
 * Initializes each scene:
 */
void scene_init() {
    objects = malloc(sizeof(object*) * 10);
    if (objects == NULL) {
        perror("Malloc error.");
        exit(1);
    }
}

/**
 * Generate a scene with various bouncing spheres:
 */
void bouncing_spheres() {
    int capacity = 10, i = 0;
    scene_init();

    texture* tex = checkered_create_from_solids(0.32, vec3_create(0.2, 0.3, .1), vec3_create(.9, .9, .9));
    material* ground = material_create(lambertian_type, lambertian_create(tex));
    objects[i++] = object_create(sphere_obj, sphere_create(vec3_create(0, -1000, 0), 1000, ground));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            // reallocate objects
            if (i >= capacity) {
                capacity *= 2;
                objects = realloc(objects, sizeof(object*) * capacity);
                if (objects == NULL) {
                    perror("Realloc error.");
                    exit(1);
                }
            }
            double choose_mat = rand_double();
            point3 center = vec3_create(a + 0.9*rand_double(), 0.2, b + 0.9*rand_double());

            if (vec3_length(vec3_sub(center, vec3_create(4, 0.2, 0))) > 0.9) {
                material* mat = malloc(sizeof(material));
                if (mat == NULL) {
                    perror("Malloc error.");
                    exit(1);
                }
                // diffuse
                if (choose_mat < 0.8) {
                    color albedo = vec3_mul(vec3_rand(), vec3_rand());
                    texture* t = texture_create(solid_tex, solid_create(albedo.x, albedo.y, albedo.z));
                    mat = material_create(lambertian_type, lambertian_create(t));
                    point3 center2 = vec3_add(center, vec3_create(0, random_double(interval_create(0, 0.5)), 0));
                    objects[i++] = object_create(moving_sphere_obj, moving_sphere_create(center, center2, 0.2, mat));
                }
                // metal
                else if (choose_mat < 0.95) {
                    color albedo = vec3_random(interval_create(0.5, 1));
                    double fuzz = random_double(interval_create(0, 0.5));
                    mat = material_create(metal_type, metal_create(albedo, fuzz));
                    objects[i++] = object_create(sphere_obj, sphere_create(center, 0.2, mat));
                }
                // dielectric
                else {
                    mat = material_create(dielectric_type, dielectric_create(1.5));
                    objects[i++] = object_create(sphere_obj, sphere_create(center, 0.2, mat));
                }
            }
        }
    }
    objects[i] = NULL;
}

/**
 * Generate a scene with various checkered spheres:
 */
void checkered_spheres() {
    scene_init();

    texture* tex = checkered_create_from_solids(0.32, vec3_create(.2, .3, .1), vec3_create(.9, .9, .9));
    material* mat0 = material_create(lambertian_type, lambertian_create(tex));
    material* mat1 = material_create(lambertian_type, lambertian_create(tex));


    objects[0] = object_create(sphere_obj, sphere_create(vec3_create(0, -10, 0), 10, mat0));
    objects[1] = object_create(sphere_obj, sphere_create(vec3_create(0, 10, 0), 10, mat1));
    objects[2] = NULL;
}

/**
 * World sphere:
 */
void world_sphere() {
    scene_init();

    // globe:
    image* img = image_create("Tx_AI_grass_dirt_01.png");
    if (img == NULL) {
        fprintf(stderr, "Failed to load earthmap.\n");
        return;
    }
    texture* tex = texture_create(image_tex, img);
    material* mat = material_create(lambertian_type, lambertian_create(tex));
    objects[0] = object_create(sphere_obj, sphere_create(vec3_create(0, 0, 0), 2, mat));
}