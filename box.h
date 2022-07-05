#pragma once

#include "common.h"
#include "aarect.h"
#include "hittable_list.h"

class box: public hittable {
public:
    point3 point_min;
    point3 point_max;
    hittable_list sides;
public:
    box() {}
    box(const point3& _p_min, const point3& _p_max, shared_ptr<material> _m_ptr)
        : point_min(_p_min), point_max(_p_max) {
        sides.add(make_shared<xy_rect>(_p_min.x(), _p_max.x(), _p_min.y(), _p_max.y(), _p_min.z(), _m_ptr));
        sides.add(make_shared<xy_rect>(_p_min.x(), _p_max.x(), _p_min.y(), _p_max.y(), _p_max.z(), _m_ptr));

        sides.add(make_shared<yz_rect>(_p_min.y(), _p_max.y(), _p_min.z(), _p_max.z(), _p_min.x(), _m_ptr));
        sides.add(make_shared<yz_rect>(_p_min.y(), _p_max.y(), _p_min.z(), _p_max.z(), _p_max.x(), _m_ptr));

        sides.add(make_shared<xz_rect>(_p_min.x(), _p_max.x(), _p_min.z(), _p_max.z(), _p_min.y(), _m_ptr));
        sides.add(make_shared<xz_rect>(_p_min.x(), _p_max.x(), _p_min.z(), _p_max.z(), _p_max.y(), _m_ptr));
    }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
        return sides.hit(r, t_min, t_max, rec);
    }

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        output_box = aabb(point_min, point_max);
        return true;
    }


};