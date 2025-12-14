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
    material* ground = lambertian_create(tex);
    objects[i++] = sphere_create(vec3_create(0, -1000, 0), 1000, ground);

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
                    mat = lambertian_create(solid_create(albedo.x, albedo.y, albedo.z));
                    point3 center2 = vec3_add(center, vec3_create(0, random_double(interval_create(0, 0.5)), 0));
                    objects[i++] = moving_sphere_create(center, center2, 0.2, mat);
                }
                // metal
                else if (choose_mat < 0.95) {
                    color albedo = vec3_random(interval_create(0.5, 1));
                    double fuzz = random_double(interval_create(0, 0.5));
                    mat = metal_create(albedo, fuzz);
                    objects[i++] = sphere_create(center, 0.2, mat);
                }
                // dielectric
                else {
                    mat = dielectric_create(1.5);
                    objects[i++] = sphere_create(center, 0.2, mat);
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

    objects[0] = sphere_create(vec3_create(0, -10, 0), 10, lambertian_create(tex));
    objects[1] = sphere_create(vec3_create(0, 10, 0), 10, lambertian_create(tex));
    objects[2] = NULL;
}

/**
 * Textured spheres:
 */
void textured_spheres() {
    scene_init();
    initialize((16.0/9.0), 400, vec3_create(0, 0, 12), vec3_create(0,0,0), 30, vec3_create(0,1,0), 0);
    c->background = vec3_create(0.7, 0.8, 1);

    // globe:
    texture* img = image_create("tx_gl_dirt_grass_02.png");
    if (img == NULL) {
        fprintf(stderr, "Failed to load earthmap.\n");
        return;
    }
    objects[0] = sphere_create(vec3_create(2, 0, 0), 2, lambertian_create(img));

    material* mat1 = lambertian_create(noise_create(4));
    objects[1] = sphere_create(vec3_create(-2, 0, 0), 2, mat1);

    objects[2] = NULL;
}

/**
 * 5 colorful quadrilaterals:
 */
void quads() {
    scene_init();
    initialize(1.0, 400, vec3_create(0, 0, 9), vec3_create(0,0,0), 80, vec3_create(0,1,0), 0);
    c->background = vec3_create(0.7, 0.8, 1);

    material* left = lambertian_create(solid_create(1, 0.2, 0.2));
    object* q1 = quad_create(vec3_create(-3, -2, 5), vec3_create(0, 0, -4), vec3_create(0, 4, 0), left);
    
    material* back = lambertian_create(solid_create(0.2, 1.0, 0.2));
    object* q2 = quad_create(vec3_create(-2, -2, 0), vec3_create(4, 0, 0), vec3_create(0, 4, 0), back);
    
    material* right = lambertian_create(solid_create(0.2, 0.2, 1.0));
    object* q3 = quad_create(vec3_create(3, -2, 1), vec3_create(0, 0, 4), vec3_create(0, 4, 0), right);
    
    material* top = lambertian_create(solid_create(1.0, 0.5, 0.0));
    object* q4 = quad_create(vec3_create(-2, 3, 1), vec3_create(4, 0, 0), vec3_create(0, 0, 4), top);
    
    material* bottom = lambertian_create(solid_create(0.2, 0.8, 0.8));
    object* q5 = quad_create(vec3_create(-2, -3, 5), vec3_create(4, 0, 0), vec3_create(0, 0, -4), bottom);

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

    object* s0 = sphere_create(vec3_create(0, -1000, 0), 1000, lambertian_create(noise_create(4)));
    object* s1 = sphere_create(vec3_create(0, 2, 0), 2, lambertian_create(noise_create(4)));
    objects[0] = s0;
    objects[1] = s1;

    object* s2 = sphere_create(vec3_create(0,7,0), 2, emissive_create_color(vec3_create(4,4,4)));
    objects[2] = s2;
    objects[3] = NULL;
}

/**
 * Triangle scene:
 */
void simple_triangle() {
    scene_init();
    initialize((16.0/9.0), 400, vec3_create(0,1,2), vec3_create(0,0,-3), 40, vec3_create(0,1,0), 0);
    c->background = vec3_create(0,0,0);

    object* ground = sphere_create(vec3_create(0,-1001,0),1000,lambertian_create(solid_create(0.8, 0.2, 0.2)));
    objects[0] = ground;

    object* tri0 = triangle_create(vec3_create(-1,0,-3), vec3_create(1,0,-3), vec3_create(0,1,-3), 
                lambertian_create(solid_create(0.8, 0.8, 0.8)));
    objects[1] = tri0;

    object* tri1 = triangle_create(vec3_create(-1,0,-3), vec3_create(1,0,-3), vec3_create(0,-1,-3),
                lambertian_create(solid_create(0.1, 0.7, 0.9)));
    objects[2] = tri1;

    object* tri2 = triangle_create(vec3_create(-2,0,-1), vec3_create(-2,0,-3), vec3_create(-2,1,-2), 
                    emissive_create_color(vec3_create(0.8, 0.2, 0.2)));
    objects[3] = tri2;

    object* tri3 = triangle_create(vec3_create(-2,0,-1), vec3_create(-2,0,-3), vec3_create(-2,-1,-2), 
                    emissive_create_color(vec3_create(1.0, 0.65, 0.0)));
    objects[4] = tri3;

    objects[5] = NULL;
}

/**
 * Final put together scene:
 */
