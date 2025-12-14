#include "bvh_node.h"

// PRIVATE FUNCTIONS

/**
 * Sphere bounding box function
 */
void sphere_box(object* o) {
    sphere* s = (sphere*) o->data;
    vec3 rvec = vec3_create(s->radius, s->radius, s->radius);
    o->bbox = aabb_points(vec3_sub(s->center.orig, rvec), vec3_add(s->center.orig, rvec));
}

/**
 * Calculates the bbox for a moving sphere:
 */
void moving_sphere_box(object* o) {
    sphere* s = (sphere*) o->data;
    vec3 center0 = at(s->center, 0);
    vec3 center1 = at(s->center, 1);
    vec3 r = vec3_create(s->radius, s->radius, s->radius);

    aabb box0 = aabb_points(vec3_sub(center0, r), vec3_add(center0, r));
    aabb box1 = aabb_points(vec3_sub(center1, r), vec3_add(center1, r));

    o->bbox = aabb_from_aabbs(box0, box1);
}

// /**
//  * Triangle bounding box function
//  */
// void triangle_box(object* o) {

// }

typedef void (*aabb_fn)(object*);
aabb_fn aabb_func[2] = {
    sphere_box,
    moving_sphere_box,
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

// PUBLIC FUNCTIONS

/**
 * Hit function array declaration:
 */
hit_fn hit_func[NUM_OBJ_TYPES] = {
    hit_sphere,
    hit_sphere,
    hit_triangle,
    hit_bvh,
};

/**
 * Create an object:
 */
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
 * Create a BVH node:
 */
bvh_node* bvh_node_create(object* left, object* right) {
    bvh_node* node = malloc(sizeof(bvh_node));
    if (node == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    node->left = left;
    node->right = right;
    node->bbox = aabb_from_aabbs(node->left->bbox, node->right->bbox);
    return node;
}


/**
 * Build the Bounding Volume Hierarchy:
 */
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

    bvh_node* node = bvh_node_create(left, right);

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

    if (o->data != NULL) {
        switch (o->type) {
            case sphere_obj:
            case moving_sphere_obj:
                if (((sphere*) o->data)->mat != NULL) {
                    if (((sphere*) o->data)->mat->data != NULL)
                            free_texture(((sphere*) o->data)->mat->data);
                    free(((sphere*) o->data)->mat);
                }
                free(o->data);
                break;
            case triangle_obj:
                free(o->data);
                break;
            case bvh_node_obj:
                if (((bvh_node*) o->data)->left != NULL)
                    free_objects(((bvh_node*) o->data)->left);
                if (((bvh_node*) o->data)->right != NULL)
                    free_objects(((bvh_node*) o->data)->right);
                free(o->data);
                break;
        }
    }
    free(o);
}