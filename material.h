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
        r_out = ray(hit_rec.p, scatter_dir);
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
        r_out = ray(hit_rec.p, scatter_dir + fuzz * random_unit_vector());
        attenuation = albedo;
        return dot(scatter_dir, hit_rec.normal) > 0;
    }
};