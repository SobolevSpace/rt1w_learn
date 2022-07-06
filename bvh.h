#pragma once

#include <algorithm>

#include "common.h"
#include "hittable.h"
#include "hittable_list.h"

class bvh_node: public hittable{
public:
    shared_ptr<hittable> left;
    shared_ptr<hittable> right;
    aabb box;
public:
    bvh_node() {}

    bvh_node(const hittable_list& h_list, double t0, double t1):
        bvh_node(h_list.objects, 0, h_list.objects.size(), t0, t1) {

    }

    bvh_node(const std::vector<shared_ptr<hittable>>& h_list, size_t start,
        size_t end, double t0, double t1);
    
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};


inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
    aabb box_a, box_b;
    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b)) {
        std::cerr<<"No bounding box in bvh_node constructor.\n";
    }
    return box_a.min().e[axis] < box_b.min().e[axis];
}

bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 2);
}

bool bvh_node::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = box;
    return true;
}

bool bvh_node::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    if (!box.hit(r, t_min, t_max)) return false;
    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left?rec.t:t_max, rec);

    return hit_left || hit_right;
}

bvh_node::bvh_node(const std::vector<shared_ptr<hittable>>& h_list, size_t start, 
size_t end, double t0, double t1) {
    std::vector<shared_ptr<hittable>> objs = h_list;

    int axis = random_int(0, 2);
    auto comparator = (axis == 0) ? box_x_compare :
                      ((axis == 1) ? box_y_compare :
                                   box_z_compare );

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objs[start];
    } else if (object_span == 2) {
        if (comparator(objs[start], objs[start+1])) {
            left = objs[start];
            right = objs[start+1];
        } else {
            left = objs[start+1];
            right = objs[start];
        }
    } else {
        std::sort(objs.begin()+start, objs.begin()+end, comparator);
        size_t mid = start + object_span/2;
        left = make_shared<bvh_node>(objs, start, mid, t0, t1);
        right = make_shared<bvh_node>(objs, mid, end, t0, t1);
    }

    aabb box_left, box_right;

    if (!left->bounding_box(t0, t1, box_left) ||
        !right->bounding_box(t0, t1, box_right)) {
            std::cerr<<"No bounding box in bvh_node constructor.\n";
    }
    box = surrounding_box(box_left, box_right);
}
