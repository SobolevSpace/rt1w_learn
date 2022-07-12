#pragma once

#include "common.h"
#include "aabb.h"

class material;

struct hit_record{
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    double u, v;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face?outward_normal:-outward_normal;
    }
};

class hittable{
public:

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;

    
    virtual double pdf_value(const point3& p, const vec3& dir) const {
        return 0.0;
    }

    virtual vec3 random(const point3& p) const {
        return vec3(0, 1, 0);
    }
};

class translate: public hittable {
public:
    shared_ptr<hittable> objptr;
    vec3 offset;
public:
    translate(shared_ptr<hittable> _objptr, const vec3& _offset)
    : objptr(_objptr), offset(_offset) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
        ray new_r(r.origin()-offset, r.direction(), r.time());
        if (!objptr->hit(new_r, t_min, t_max, rec)) {
            return false;
        }
        rec.p += offset;
        rec.set_face_normal(new_r, rec.normal);
        return true;
    }

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        if (!objptr->bounding_box(time0, time1, output_box)) {
            return false;
        }
        output_box = aabb(
            output_box.min() + offset,
            output_box.max() + offset
        );
        return true;
    }

};

class rotate_y: public hittable {
public:
    shared_ptr<hittable> objptr;
    double sin_theta, cos_theta;
    aabb bbox;
    bool hasbox;
public:
    rotate_y(shared_ptr<hittable> _objptr, double angle);
    
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        output_box = bbox;
        return hasbox;
    }
};

class flip_face: public hittable {
public:
    shared_ptr<hittable> objptr;
public:
    flip_face(shared_ptr<hittable> _objptr): objptr(_objptr) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {

        if (!objptr->hit(r, t_min, t_max, rec)) {
            return false;
        }
        rec.front_face = !rec.front_face;
        return true;
    }

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        return objptr->bounding_box(time0, time1, output_box);
    }
};

rotate_y::rotate_y(shared_ptr<hittable> _objptr, double angle): objptr(_objptr) {
    angle = degree2radians(angle);
    sin_theta = std::sin(angle);
    cos_theta = std::cos(angle);
    hasbox = objptr->bounding_box(0, 1, bbox);

    point3 pmin(INF, INF, INF), pmax(-INF, -INF, -INF);

    for (int i=0;i<2;i++) {
        for (int j=0;j<2;j++) {
            for (int k=0;k<2;k++) {
                //traverse the 8 point of the box
                double x = i*bbox.max().x() + (1-i)*bbox.min().x();
                double y = j*bbox.max().y() + (1-j)*bbox.min().y();
                double z = k*bbox.max().z() + (1-k)*bbox.min().z();

                double nx = x * cos_theta + z * sin_theta;
                double nz = -sin_theta * x + cos_theta * z;

                vec3 tester(nx, y, nz);
                for (int t=0;t<3;t++) {
                    pmin[t] = fmin(pmin[t], tester[t]);
                    pmax[t] = fmax(pmax[t], tester[t]);
                }
            }
        }
    }
    bbox = aabb(pmin, pmax);
}

bool rotate_y::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    ray new_r = r;
    new_r.orig[0] = cos_theta*r.orig[0]-sin_theta*r.orig[2];
    new_r.orig[2] = sin_theta*r.orig[0]+cos_theta*r.orig[2];
    new_r.dir[0] = cos_theta*r.dir[0]-sin_theta*r.dir[2];
    new_r.dir[2] = sin_theta*r.dir[0]+cos_theta*r.dir[2];

    if (!objptr->hit(new_r, t_min, t_max, rec)) {
        return false;
    }

    vec3 p = rec.p, normal = rec.normal;
    rec.p[0] = cos_theta*p[0]+sin_theta*p[2];
    rec.p[2] = -sin_theta*p[0]+cos_theta*p[2];
    rec.normal[0] = cos_theta*normal[0]+sin_theta*normal[2];
    rec.normal[2] = -sin_theta*normal[0]+cos_theta*normal[2];
    rec.set_face_normal(r, rec.normal);

    return true;
}
