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
