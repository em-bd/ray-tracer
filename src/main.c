#include "scenes.h"

int main(int argc, char* argvc[]) {

    // Generate objects
    if (argc == 1) {
        printf("Generating bouncing spheres.\n");
        bouncing_spheres();
    }
    else if (argc > 1) {
        int input = atoi(argvc[1]);
        switch (input) {
            case 2:
                printf("Generating checkered spheres.\n");
                checkered_spheres();
                break;
            case 3:
                printf("Generating textured spheres.\n");
                textured_spheres();
                break;
            case 4:
                printf("Generating colorful quads.\n");
                quads();
                break;
            case 5:
                printf("Generating a simple light scene.\n");
                simple_light();
                break;
            case 6:
                printf("Generating a simple triangle scene.\n");
                simple_triangle();
                break;
            default:
                printf("Generating bouncing spheres.\n");
                bouncing_spheres();
                break;
        }
    }

    int i = 0;
    while (objects[i] != NULL) ++i;
    world = build_bvh(objects, 0, i);
    // Render
    render();
    printf("Done!\n");

    return 0;
}