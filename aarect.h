#pragma once

#include "common.h"
#include "hittable.h"

class xy_rect: public hittable{
public:
    shared_ptr<material> mat_ptr;
    double x0, x1, y0, y1, k;

public:
    xy_rect() {}
    xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<material> _mat_ptr):
        x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mat_ptr(_mat_ptr) {}
    
    
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        output_box = aabb(point3(x0, y0, k-1e-4), point3(x1, y1, k+1e-4));
        return true;
    }
};

bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    double t = (k-r.origin().z())/r.direction().z();
    if (std::isnan(t) || t<t_min || t>t_max) {
        return false;
    }
    point3 p = r.at(t);
    double x = p.x(), y = p.y();
    if (x<x0||x>x1||y<y0||y>y1) {
        return false;
    }
    rec.u = (x-x0)/(x1-x0);
    rec.v = (y-y0)/(y1-y0);
    rec.t = t;
    rec.p = p;
    rec.mat_ptr = mat_ptr;
    vec3 outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    return true;
}