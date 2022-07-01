#pragma once

#include "common.h"
#include "hittable.h"

struct hit_record;

class material{
public:
    virtual bool scatter(
        const ray& r_in, const hit_record& hit_rec, color& attenuation, ray& r_out
    ) const = 0;
};

class lambertian: public material {
public:
    color albedo;
public:
    lambertian(color& _albedo): albedo(_albedo) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& hit_rec, color& attenuation, ray& r_out
    ) const override {
        vec3 scatter_dir = hit_rec.normal + random_unit_vector();
        if (scatter_dir.near_zero()) scatter_dir = hit_rec.normal;
        r_out = ray(hit_rec.p, scatter_dir, r_in.time());
        attenuation = albedo;
        return true;
    }
};

class metal: public material {
public:
    color albedo;
    double fuzz;
public:
    metal(color& _albedo, double _fuzz): albedo(_albedo), fuzz(_fuzz<1?_fuzz:1) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& hit_rec, color& attenuation, ray& r_out
    ) const override {
        vec3 scatter_dir = reflect(unit_vector(r_in.direction()), hit_rec.normal);
        r_out = ray(hit_rec.p, scatter_dir + fuzz * random_unit_vector(), r_in.time());
        attenuation = albedo;
        return dot(scatter_dir, hit_rec.normal) > 0;
    }
};

class dielectric: public material {
public:
    double ir;
public:
    dielectric(double _ir): ir(_ir) {}
    virtual bool scatter(
        const ray& r_in, const hit_record& hit_rec, color& attenuation, ray& r_out
    ) const override {
        attenuation = color(1.0, 1.0, 1.0);
        double refract_ratio = hit_rec.front_face?(1.0/ir):ir;
        vec3 unit_v = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_v, hit_rec.normal), 1.0);
        double sin_theta = sqrt(1 - cos_theta*cos_theta);
        vec3 scatter_dir;
        if (sin_theta * refract_ratio > 1.0 || reflectance(cos_theta, refract_ratio) > random_double()) {
            scatter_dir = reflect(unit_v, hit_rec.normal);
        } else {
            scatter_dir = refract(unit_vector(r_in.direction()), hit_rec.normal, refract_ratio);
        }
        r_out = ray(hit_rec.p, scatter_dir, r_in.time());
        return true;
    }

private:
    static double reflectance(double cosine, double ref_idx) {
        double r0 = (1-ref_idx)/(1+ref_idx);
        r0 *= r0;
        return r0 + (1-r0) * pow(1-cosine, 5);
    }
};