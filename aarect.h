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

class xz_rect: public hittable{
public:
    shared_ptr<material> mat_ptr;
    double x0, x1, z0, z1, k;

public:
    xz_rect() {}
    xz_rect(double _x0, double _x1, double _z0, double _z1, double _k, shared_ptr<material> _mat_ptr):
        x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mat_ptr(_mat_ptr) {}
    
    
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        output_box = aabb(point3(x0, k-1e-4, z0), point3(x1, k+1e-4, z1));
        return true;
    }

    
    virtual double pdf_value(const point3& p, const vec3& dir) const override{
        hit_record hit_rec;
        if (!this->hit(ray(p, dir, 0), 0.0001, INF, hit_rec)) {
            return 0;
        }
        double dist2 = dir.length_sqr()*hit_rec.t*hit_rec.t;
        double area = (z1-z0)*(x1-x0);
        double costheta = fabs(dot(hit_rec.normal, unit_vector(dir)));
        return dist2/(area*costheta);
    }

    virtual vec3 random(const point3& p) const override {
        vec3 rand_in_rect = vec3(random_double(x0, x1), k, random_double(z0, z1));
        return rand_in_rect - p;
    }
};

bool xz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    double t = (k-r.origin().y())/r.direction().y();
    if (std::isnan(t) || t<t_min || t>t_max) {
        return false;
    }
    point3 p = r.at(t);
    double x = p.x(), z = p.z();
    if (x<x0||x>x1||z<z0||z>z1) {
        return false;
    }
    rec.u = (x-x0)/(x1-x0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    rec.p = p;
    rec.mat_ptr = mat_ptr;
    vec3 outward_normal = vec3(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    return true;
}

class yz_rect: public hittable{
public:
    shared_ptr<material> mat_ptr;
    double y0, y1, z0, z1, k;

public:
    yz_rect() {}
    yz_rect(double _y0, double _y1, double _z0, double _z1, double _k, shared_ptr<material> _mat_ptr):
        y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mat_ptr(_mat_ptr) {}
    
    
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        output_box = aabb(point3(k-1e-4, y0, z0), point3(k+1e-4, y1, z1));
        return true;
    }
};

bool yz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    double t = (k-r.origin().x())/r.direction().x();
    if (std::isnan(t) || t<t_min || t>t_max) {
        return false;
    }
    point3 p = r.at(t);
    double y = p.y(), z = p.z();
    if (y<y0||y>y1||z<z0||z>z1) {
        return false;
    }
    rec.u = (y-y0)/(y1-y0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    rec.p = p;
    rec.mat_ptr = mat_ptr;
    vec3 outward_normal = vec3(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    return true;
}
