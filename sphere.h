#pragma once

#include "hittable.h"

class sphere: public hittable {
public:
    vec3 center;
    double radius;
    shared_ptr<material> mat_ptr;

public:
    sphere():radius(0) {}
    sphere(const point3& _center, double r, shared_ptr<material> m_ptr):center(_center), radius(r), mat_ptr(m_ptr) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};


bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    point3 nt = r.origin() - center;
    //(nt.x() + t * r.direction().x ) ^ 2 == radius ^ 2
    // t^2 * r.direction().length_sqr() + t* 2*(dot (r.dir, nt)) + nt.length_sqr - r^2 = 0;
    double A = r.direction().length_sqr(), B = 2*dot(r.direction(), nt), C = nt.length_sqr() - radius*radius;
    double delta = B*B - 4*A*C;
    if (delta < 0) {
        return false;
    }
    double sqrtd = sqrt(delta);
    double root = (-B-sqrtd)/(2*A);
    if (root > t_max || root < t_min) {
        root = (-B+sqrtd)/(2*A);
        if (root > t_max || root < t_min) {
            return false;
        }
    }
    rec.t = root;
    rec.p = r.at(root);
    vec3 out_normal = (rec.p - center)/radius;
    rec.set_face_normal(r, out_normal);
    rec.mat_ptr = mat_ptr;
    return true;
}