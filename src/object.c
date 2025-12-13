#include "object.h"

/**
 * Determine if the ray hits the sphere:
 */
bool hit_sphere(ray r, interval i, hit_record* rec, object* o) {
    sphere* s = (sphere *) o->data;
    vec3 oc = vec3_sub(s->center, r.orig);
    double a = length_sqd(r.dir);
    double h = vec3_dot(r.dir, oc);
    double c = length_sqd(oc) - s->radius * s->radius;

    double discriminant = h*h - a*c;
    if (discriminant < 0)
        return false;

    double sqrtd = sqrt(discriminant);

    double root = (h - sqrtd) / a;
    // check if the root is in the acceptable range:
    if (!surrounds(i, root)) {
        root = (h + sqrtd) / a;
        if (!surrounds(i, root))
            return false;
    }

    // update hit record:
    rec->t = root;
    rec->p = at(r, rec->t);
    vec3 outward_normal = vec3_scalar(vec3_sub(rec->p, s->center), (1.0 / s->radius));
    set_face_normal(r, outward_normal, rec);
    rec->mat = ((sphere*) o->data)->mat;

    return true;
}

/**
 * Determine if the ray hits the triangle:
 */
bool hit_triangle(ray r, interval i, hit_record* rec, object* o) {
    //TODO: complete triangle hit calculation
    return true;
}

/**
 * Determine if the ray hits the BVH node:
 */
bool hit_bvh(ray r, interval i, hit_record* rec, object* o) {
    bvh_node* bvh = (bvh_node*) o->data;
    hit_record left_rec, right_rec;

    if (!aabb_hit(r, i, bvh->bbox))
        return false;

    bool hit_left = bvh->left && hit_func[bvh->left->type](r, i, &left_rec, bvh->left);
    interval right_range = i;
    if (hit_left) right_range.max = left_rec.t;
    bool hit_right = bvh->right && hit_func[bvh->right->type](r, right_range, &right_rec, bvh->right);

    if (hit_left && hit_right)
        *rec = (right_rec.t < left_rec.t) ? right_rec : left_rec;
    else if (hit_left)
        *rec = left_rec;
    else if (hit_right)
        *rec = right_rec;

    return hit_left || hit_right;
}

/**
 * Hit function array declaration:
 */
hit_fn hit_func[3] = {
    hit_sphere,
    hit_triangle,
    hit_bvh,
};

/**
 * Sets the normal to face outward from the surface:
 */
void set_face_normal(ray r, vec3 outward_normal, hit_record* rec) {
    rec->front_face = vec3_dot(r.dir, outward_normal) < 0;
    rec->normal = rec->front_face ? outward_normal : vec3_negative(outward_normal);
}

// HELPER FUNCTIONS

/**
 * Sphere bounding box function
 */
void sphere_box(object* o) {
    sphere* s = (sphere*) o->data;
    vec3 rvec = vec3_create(s->radius, s->radius, s->radius);
    o->bbox = aabb_points(vec3_sub(s->center, rvec), vec3_add(s->center, rvec));
}

// /**
//  * Triangle bounding box function
//  */
// void triangle_box(object* o) {

// }

typedef void (*aabb_fn)(object*);
aabb_fn aabb_func[2] = {
    sphere_box,
    // triangle_box,
};

// comparator functions:
int box_x_compare(const object* a, const object* b) {
    return a->bbox.x.min < b->bbox.x.min ? -1 : (a->bbox.x.min > b->bbox.x.min ? 1 : 0);
}

int box_y_compare(const object* a, const object* b) {
    return a->bbox.y.min < b->bbox.y.min ? -1 : (a->bbox.y.min > b->bbox.y.min ? 1 : 0);
}

int box_z_compare(const object* a, const object* b) {
    return a->bbox.z.min < b->bbox.z.min ? -1 : (a->bbox.z.min > b->bbox.z.min ? 1 : 0);
}

typedef int (*cmp_fn)(const object*, const object*);
cmp_fn comp_fn[3] = {
    box_x_compare,
    box_y_compare,
    box_z_compare,
};

void merge(object** objs, size_t left, size_t mid, size_t right, cmp_fn comp) {
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;

    object** L = malloc(sizeof(object*) * n1);
    object** R = malloc(sizeof(object*) * n2);

    for (size_t i = 0; i < n1; i++) L[i] = objs[left + i];
    for (size_t j = 0; j < n2; j++) R[j] = objs[mid + 1 + j];

    size_t i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if(comp(L[i], R[j]) <= 0)
            objs[k++] = L[i++];
        else
            objs[k++] = R[j++];
    }

    // add the rest in:
    while (i < n1) objs[k++] = L[i++];
    while (j < n2) objs[k++] = R[j++];

    free(L);
    free(R);
}

void stable_mergesort(object** objs, size_t left, size_t right, cmp_fn comp) {
    if (left < right) {
        size_t mid = left + (right - left) / 2;
        stable_mergesort(objs, left, mid, comp);
        stable_mergesort(objs, mid + 1, right, comp);
        merge(objs, left, mid, right, comp);
    }
}

// CREATION FUNCTIONS

object* object_create(obj_type type, void* data) {
    object* o = malloc(sizeof(object));
    if (o == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    o->type = type;
    o->data = data;
    // determine which bounding box to assign to this object:
    aabb_func[o->type](o);
    return o;
}

/**
 * Create a sphere:
 */
object* sphere_create(vec3 center, double radius, material* mat) {
    sphere* s = malloc(sizeof(sphere));
    if (s == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    s->center = center;
    s->radius = radius;
    s->mat = mat;
    
    return object_create(sphere_obj, s);
}

/**
 * Create a BVH node:
 */
bvh_node bvh_node_create(object* left, object* right) {
    bvh_node bvh;
    bvh.left = left;
    bvh.right = right;
    bvh.bbox = aabb_from_aabbs(left->bbox, right->bbox);
    return bvh;
}

object* build_bvh(object** objs, size_t start, size_t end) {
    printf("Building a node.\n");
    size_t object_span = end - start;
    if (object_span == 1)
        return objs[start];

    aabb bbox = empty_aabb;
    // build bounding box for the span of objects:
    for (size_t i=start; i < end; i++)
        bbox = aabb_from_aabbs(bbox, objs[i]->bbox);

    int axis = longest_axis(bbox);
    object* left;
    object* right;

    stable_mergesort(objs + start, 0, object_span - 1, comp_fn[axis]);

    size_t mid = start + object_span / 2;
    left = build_bvh(objs, start, mid);
    right = build_bvh(objs, mid, end);

    bvh_node* node = malloc(sizeof(bvh_node));
    if (node == NULL) {
        perror("Malloc error");
        exit(1);
    }
    *node = bvh_node_create(left, right);

    object* o = malloc(sizeof(object));
    if (o == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    o->data = node;
    o->type = bvh_node_obj;
    o->bbox = aabb_from_aabbs(left->bbox, right->bbox);

    printf("Done building node.\n");
    return o;
}

/**
 * Recursively free objects:
 */
void free_objects(object* o) {
    if (o == NULL)
        return;

    switch (o->type) {
        case sphere_obj:
            if (o->data != NULL) {
                if (((sphere*) o->data)->mat != NULL)
                    free(((sphere*) o->data)->mat);
                free(o->data);
            }
            break;
        case triangle_obj:
            if (o->data != NULL)
                free(o->data);
            break;
        case bvh_node_obj:
            if (o->data != NULL) {
                if (((bvh_node*) o->data)->left != NULL)
                    free_objects(((bvh_node*) o->data)->left);
                if (((bvh_node*) o->data)->right != NULL)
                    free_objects(((bvh_node*) o->data)->right);
                free(o->data);
            }
            break;
    }
    free(o);
}