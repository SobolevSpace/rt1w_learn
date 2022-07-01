#pragma once

#include "common.h"
#include "hittable.h"

class moving_sphere: public hittable {
public:
    point3 center0, center1;
    double time0, time1;
    double radius;
    shared_ptr<material> mat_ptr;
public:
    moving_sphere() {}

    moving_sphere(
        const point3& _c0, const point3& _c1, double _t0, double _t1, double _r, shared_ptr<material> m
    ):center0(_c0), center1(_c1), time0(_t0), time1(_t1), radius(_r), mat_ptr(m) {

    }

    point3 center(double t) const {
        return center0 + ((t-time0)/(time1-time0)) * (center1-center0);
    }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
};

bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    point3 nt = r.origin() - center(r.time());
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
    vec3 out_normal = (rec.p - center(r.time()))/radius;
    rec.set_face_normal(r, out_normal);
    rec.mat_ptr = mat_ptr;
    return true;
}


bool moving_sphere::bounding_box(double _time0, double _time1, aabb& output_box) const {
    aabb abox = aabb(
        center(_time0)-vec3(radius, radius, radius),
        center(_time0)+vec3(radius, radius, radius));
    aabb bbox = aabb(
        center(_time1)-vec3(radius, radius, radius),
        center(_time1)+vec3(radius, radius, radius)
    );
    output_box = surrounding_box(abox, bbox);
    return true;
}