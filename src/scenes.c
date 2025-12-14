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
    initialize((16.0/9.0), 400, vec3_create(13,2,3), vec3_create(0,0,0), 20, vec3_create(0,1,0), 0.6);
    c->background = vec3_create(0.7, 0.8, 1);

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
    initialize((16.0/9.0), 400, vec3_create(13,2,3), vec3_create(0,0,0), 20, vec3_create(0,1,0), 0);
    c->background = vec3_create(0.7, 0.8, 1);

    texture* tex = checkered_create_from_solids(0.32, vec3_create(.2, .3, .1), vec3_create(.9, .9, .9));
    material* mat0 = material_create(lambertian_type, lambertian_create(tex));
    material* mat1 = material_create(lambertian_type, lambertian_create(tex));


    objects[0] = object_create(sphere_obj, sphere_create(vec3_create(0, -10, 0), 10, mat0));
    objects[1] = object_create(sphere_obj, sphere_create(vec3_create(0, 10, 0), 10, mat1));
    objects[2] = NULL;
}

/**
 * Textured spheres:
 */
void textured_spheres() {
    scene_init();
    initialize((16.0/9.0), 400, vec3_create(0, 0, 12), vec3_create(0,0,0), 20, vec3_create(0,1,0), 0);
    c->background = vec3_create(0.7, 0.8, 1);

    // globe:
    image* img = image_create("Tx_AI_grass_dirt_01.png");
    if (img == NULL) {
        fprintf(stderr, "Failed to load earthmap.\n");
        return;
    }
    texture* tex0 = texture_create(image_tex, img);
    material* mat0 = material_create(lambertian_type, lambertian_create(tex0));
    objects[0] = object_create(sphere_obj, sphere_create(vec3_create(2, 0, 0), 2, mat0));

    texture* tex1 = texture_create(noise_tex, noise_create(4));
    material* mat1 = material_create(lambertian_type, lambertian_create(tex1));
    objects[1] = object_create(sphere_obj, sphere_create(vec3_create(-2, 0, 0), 2, mat1));

    objects[2] = NULL;
}

/**
 * 5 colorful quadrilaterals:
 */
void quads() {
    scene_init();
    initialize(1.0, 400, vec3_create(0, 0, 9), vec3_create(0,0,0), 80, vec3_create(0,1,0), 0);
    c->background = vec3_create(0.7, 0.8, 1);

    texture* red = texture_create(solid_tex, solid_create(1, 0.2, 0.2));
    material* left = material_create(lambertian_type, lambertian_create(red));
    object* q1 = object_create(quad_obj, quad_create(vec3_create(-3, -2, 5), vec3_create(0, 0, -4), vec3_create(0, 4, 0), left));
    
    texture* green = texture_create(solid_tex, solid_create(0.2, 1.0, 0.2));
    material* back = material_create(lambertian_type, lambertian_create(green));
    object* q2 = object_create(quad_obj, quad_create(vec3_create(-2, -2, 0), vec3_create(4, 0, 0), vec3_create(0, 4, 0), back));
    
    texture* blue = texture_create(solid_tex, solid_create(0.2, 0.2, 1.0));
    material* right = material_create(lambertian_type, lambertian_create(blue));
    object* q3 = object_create(quad_obj, quad_create(vec3_create(3, -2, 1), vec3_create(0, 0, 4), vec3_create(0, 4, 0), right));
    
    texture* orange = texture_create(solid_tex, solid_create(1.0, 0.5, 0.0));
    material* top = material_create(lambertian_type, lambertian_create(orange));
    object* q4 = object_create(quad_obj, quad_create(vec3_create(-2, 3, 1), vec3_create(4, 0, 0), vec3_create(0, 0, 4), top));
    
    texture* teal = texture_create(solid_tex, solid_create(0.2, 0.8, 0.8));
    material* bottom = material_create(lambertian_type, lambertian_create(teal));
    object* q5 = object_create(quad_obj, quad_create(vec3_create(-2, -3, 5), vec3_create(4, 0, 0), vec3_create(0, 0, -4), bottom));

    objects[0] = q1;
    objects[1] = q2;
    objects[2] = q3;
    objects[3] = q4;
    objects[4] = q5;
    objects[5] = NULL;
}

void simple_light() {
    scene_init();
    initialize((16.0/9.0), 400, vec3_create(26,3,6), vec3_create(0,2,0), 20, vec3_create(0,1,0), 0);
    c->background = vec3_create(0,0,0);

    texture* pertext = texture_create(noise_tex, noise_create(4));
    object* s0 = object_create(sphere_obj, sphere_create(vec3_create(0, -1000, 0), 1000, material_create(lambertian_type, lambertian_create(pertext))));
    object* s1 = object_create(sphere_obj, sphere_create(vec3_create(0, 2, 0), 2, material_create(lambertian_type, lambertian_create(pertext))));
    objects[0] = s0;
    objects[1] = s1;

    material* difflight = material_create(emissive_type, emissive_create_color(vec3_create(4,4,4)));
    object* s2 = object_create(sphere_obj, sphere_create(vec3_create(0,7,0), 2, difflight));
    objects[2] = s2;
    objects[3] = NULL;
}