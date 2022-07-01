#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

class hittable_list: public hittable{
public:
    std::vector<std::shared_ptr<hittable>> objects;
public:
    hittable_list() {}
    hittable_list(std::shared_ptr<hittable> object) {
        add(object);
    }

    void clear() {
        objects.clear();
    }

    void add(std::shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record rec_temp;
    double closet_so_far = t_max;
    bool hit_any = false;

    for (const auto& obj:objects) {
        if (obj->hit(r, t_min, closet_so_far, rec_temp)) {
            hit_any = true;
            closet_so_far = rec_temp.t;
            rec = rec_temp;
        }
    }
    return hit_any;
}


bool hittable_list::bounding_box(double time0, double time1, aabb& output_box) const {
    if (objects.empty()) return false;

    bool first_obj = true;
    aabb temp_box;
    for (auto object:objects) {
        if (!object->bounding_box(time0, time1, temp_box)) return false;
        if (first_obj) output_box = temp_box;
        else output_box = surrounding_box(output_box, temp_box);
        first_obj = false;
    }
    return true;
}