#include "bvh_node.h"

// PRIVATE FUNCTIONS

// comparator functions:
int box_x_compare(const void* a, const void* b) {
    return (*(const object* const*) a)->bbox.x.min < (*(const object* const*) b)->bbox.x.min ? -1 :
            ((*(const object* const*) a)->bbox.x.min > (*(const object* const*) b)->bbox.x.min ? 1 : 0);
}

int box_y_compare(const void* a, const void* b) {
    return (*(const object* const*) a)->bbox.y.min < (*(const object* const*) b)->bbox.y.min ? -1 :
            ((*(const object* const*) a)->bbox.y.min > (*(const object* const*) b)->bbox.y.min ? 1 : 0);
}

int box_z_compare(const void* a, const void* b) {
    return (*(const object* const*) a)->bbox.z.min < (*(const object* const*) b)->bbox.z.min ? -1 :
            ((*(const object* const*) a)->bbox.z.min > (*(const object* const*) b)->bbox.z.min ? 1 : 0);
}

typedef int (*cmp_fn)(const void*, const void*);
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
bool hit_bvh(ray r, interval i, hit_record* rec, void* o) {
    bvh_node* bvh = ((bvh_node*) ((object*) o)->data);
    hit_record left_rec, right_rec;

    if (!aabb_hit(r, i, bvh->bbox))
        return false;

    bool hit_left = bvh->left && bvh->left->hit(r, i, &left_rec, bvh->left);
    interval right_range = i;
    if (hit_left) right_range.max = left_rec.t;
    bool hit_right = bvh->right && bvh->right->hit(r, right_range, &right_rec, bvh->right);

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
 * Create a BVH node:
 */
object* bvh_node_create(object* left, object* right) {
    bvh_node* node = malloc(sizeof(bvh_node));
    if (node == NULL) {
        perror("Malloc error.");
        exit(1);
    }
    node->left = left;
    node->right = right;
    node->bbox = aabb_from_aabbs(node->left->bbox, node->right->bbox);
    return object_create(bvh_node_obj, node, node->bbox, hit_bvh);
}

/**
 * Build the Bounding Volume Hierarchy:
 */
object* build_bvh(object** objs, size_t start, size_t end) {
    if (end <= start) {
        fprintf(stderr, "Invalid range start=%zu, end=%zu.\n", start, end);
        exit(1);
    }

    size_t object_span = end - start;
    if (object_span == 1) {
        if (objs[start] == NULL) {
            fprintf(stderr, "BVH leaf is NULL at index %zu.", start);
            exit(1);
        }
        return objs[start];
    }

    aabb bbox = empty_aabb;
    // build bounding box for the span of objects:
    for (size_t i=start; i < end; i++) {
        if (objs[i] == NULL) {
            fprintf(stderr, "NULL object in BVH at index %zu.\n", i);
            exit(1);
        }
        bbox = aabb_from_aabbs(bbox, objs[i]->bbox);
    }

    int axis = longest_axis(bbox);
    object* left;
    object* right;

    // stable_mergesort(objs + start, 0, object_span - 1, comp_fn[axis]);
    qsort(objs + start, object_span, sizeof(object*), comp_fn[axis]);

    size_t mid = start + object_span / 2;
    left = build_bvh(objs, start, mid);
    right = build_bvh(objs, mid, end);

    object* node = bvh_node_create(left, right);
    return node;
}

/**
 * Recursively free objects:
 */
void free_objects(object* o) {
    if (o == NULL)
        return;

    if (o->data != NULL) {
        switch (o->type) {
            case bvh_node_obj:
                if (((bvh_node*) o->data)->left != NULL)
                    free_objects(((bvh_node*) o->data)->left);
                if (((bvh_node*) o->data)->right != NULL)
                    free_objects(((bvh_node*) o->data)->right);
                free(o->data);
                break;
            default:
                free(o->data);
                break;
        }
    }
    free(o);
}