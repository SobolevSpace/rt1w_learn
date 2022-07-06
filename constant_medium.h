#pragma once

#include "common.h"
#include "hittable.h"
#include "material.h"
#include "texture.h"

class constant_medium: public hittable {
public:
    shared_ptr<hittable> boundary;
    shared_ptr<material> phase_function;
    double neg_inv_density;
public:
    constant_medium(shared_ptr<hittable> _boundary, double _density, shared_ptr<texture> _phase_func)
    : boundary(_boundary), phase_function(make_shared<isotropic>(_phase_func)), neg_inv_density(-1/_density) {}

    constant_medium(shared_ptr<hittable> _boundary, double _density, color _color)
    : boundary(_boundary), phase_function(make_shared<isotropic>(_color)), neg_inv_density(-1/_density) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        return boundary->bounding_box(time0, time1, output_box);
    }
};

bool constant_medium::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    const bool enableDebug = false;
    const bool debugging = enableDebug && random_double() < 0.00001;
    hit_record rec1, rec2;

    if (!boundary->hit(r, -INF, INF, rec1)) {
        return false;
    }
    if (!boundary->hit(r, rec1.t+1e-4, INF, rec2)) {
        return false;
    }

    if (debugging) {
        std::cerr<<"t_min: "<<rec1.t<<"\tt_max: "<<rec2.t<<"\n";
    }

    if (rec1.t < t_min) rec1.t = t_min;
    if (rec2.t > t_max) rec2.t = t_max;
    if (rec1.t >= rec2.t) return false;
    if (rec1.t < 0) rec1.t = 0;

    double ray_dist = r.direction().length();
    double inside_dist = ray_dist * (rec2.t - rec1.t);
    double hit_dist = neg_inv_density * log(random_double());

    if (hit_dist > inside_dist) return false;

    rec.t = rec1.t + hit_dist/ray_dist;
    rec.p = r.at(rec.t);

    if (debugging) {
        std::cerr << "hit_distance = " <<  hit_dist << '\n'
        << "rec.t = " <<  rec.t << '\n'
        << "rec.p = " <<  rec.p << '\n';
    }

    rec.normal = vec3(1, 0, 0);
    rec.front_face = true;
    rec.mat_ptr = phase_function;

    return true;
}