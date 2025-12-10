#include "object.h"

double hit_sphere(ray r, sphere s) {
    vec3 oc = vec3_sub(s.center, r.orig);
    double a = vec3_dot(r.dir,  r.dir);
    double b = -2.0 * vec3_dot(r.dir, oc);
    double c = vec3_dot(oc, oc) - s.radius*s.radius;
    double discriminant = b*b - 4*a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant) ) / (2.0 * a);
    }
}

// /**
//  * Determine if the ray hits the sphere:
//  */
// bool hitSphere(ray* r, double ray_tmin, double ray_tmax, hit_record* rec, sphere s) {
//     // ray-sphere function:
//     vec3 oc;
//     vec3_sub(oc, s->center, r->orig);

//     double a = length_sqd(r.dir);
//     double h = vec3_dot(r.dir, oc);
//     double c = (length_sqd(oc)) - s->radius * s->radius;

//     double discriminant = h*h - a*c;

//     double sqrtd = sqrt(discriminant);

//     double root = (h - sqrtd) / a;
//     // check if the root is in the acceptable range:
//     if (root <= ray_tmin || ray_tmax <= root) {
//         root = (h + sqrtd) / a;
//         if (root <= ray_tmin || ray_tmax <= root) {
//             if (oc != NULL)
//                 free(oc);
//             return false;
//         }
//     }

//     // update hit record:
//     rec->t = root;

//     if (rec->p != NULL)
//         free(rec->p);
//     rec->p = at(r, rec->t);
//     vec3* outward_normal = malloc(sizeof(vec3));
//     vec3_sub(outward_normal, *rec->p, *s.center);
//     vec3_scalar(outward_normal, *outward_normal, (1.0 / s.radius));
//     set_face_normal(r, outward_normal, rec);
//     if (rec->front_face == false)
//         free(outward_normal);

//     if (oc != NULL)
//         free(oc);

//     return true;
// }

// /**
//  * Sets the normal to face outward from the surface:
//  */
// void set_face_normal(ray r, vec3* outward_normal, hit_record *rec) {
//     rec->front_face = vec3_dot(*r.dir, *outward_normal) < 0;
//     vec3 v;
//     vec3_negative(&v, *outward_normal);
//     if (rec->normal != NULL)
//         free(rec->normal);
//     rec->normal = rec->front_face ? outward_normal : &v;
// }