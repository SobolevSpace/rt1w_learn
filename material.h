#pragma once

#include "common.h"
#include "hittable.h"
#include "texture.h"
#include "onb.h"
#include "pdf.h"

struct hit_record;

struct scatter_record{
    ray specular_ray;
    bool is_specular;
    color attenuation;
    shared_ptr<pdf> pdf_ptr;
};

class material{
public:
    virtual bool scatter(
        const ray& r_in, const hit_record& hit_rec, scatter_record& rec
    ) const {
        return false;
    }

    virtual double scattering_pdf(
        const ray& r_in, const hit_record& hit_rec, const ray& r_out
    ) const {
        return 0.0;
    }

    virtual color emitted(
        const ray& r_in, const hit_record& hit_rec, double u, double v, const vec3& p
    ) const {
        return color(0, 0, 0);
    }

};

class lambertian: public material {
public:
    shared_ptr<texture> albedo;
public:
    lambertian(const color& _color): albedo(make_shared<solid_color>(_color)) {}
    lambertian(shared_ptr<texture> _albedo): albedo(_albedo) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& hit_rec, scatter_record& rec
    ) const override {
        rec.is_specular = false;
        rec.attenuation = albedo->value(hit_rec.u, hit_rec.v, hit_rec.p);
        shared_ptr<pdf> tp(new cosine_pdf(hit_rec.normal));
        rec.pdf_ptr = std::move(tp);
        return true;
    }

    virtual double scattering_pdf(
        const ray& r_in, const hit_record& hit_rec, const ray& r_out
    ) const {
        double cosine = dot(unit_vector(r_out.direction()), hit_rec.normal);
        return cosine > 0.0? cosine/PI: 0.0;
    }
};

class metal: public material {
public:
    color albedo;
    double fuzz;
public:
    metal(color& _albedo, double _fuzz): albedo(_albedo), fuzz(_fuzz<1?_fuzz:1) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& hit_rec, scatter_record& srec
    ) const override {
        srec.is_specular = true;
        vec3 reflected = reflect(unit_vector(r_in.direction()), hit_rec.normal);
        srec.specular_ray = ray(hit_rec.p, reflected+fuzz*random_in_unit_sphere(), r_in.time());
        srec.attenuation = albedo;
        srec.pdf_ptr = 0;
        return true;
    }
};

class dielectric: public material {
public:
    double ir;
public:
    dielectric(double _ir): ir(_ir) {}
    virtual bool scatter(
        const ray& r_in, const hit_record& hit_rec, scatter_record& srec
    ) const override {
        srec.attenuation = color(1.0, 1.0, 1.0);
        srec.is_specular = true;
        srec.pdf_ptr = nullptr;
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
        srec.specular_ray = ray(hit_rec.p, scatter_dir, r_in.time());
        return true;
    }

private:
    static double reflectance(double cosine, double ref_idx) {
        double r0 = (1-ref_idx)/(1+ref_idx);
        r0 *= r0;
        return r0 + (1-r0) * pow(1-cosine, 5);
    }
};

class diffuse_light: public material {
public:
    shared_ptr<texture> emit;
public:
    diffuse_light(color _color): emit(make_shared<solid_color>(_color)) {}
    diffuse_light(shared_ptr<texture> _emit): emit(_emit) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& hit_rec, scatter_record& srec
    ) const override {
        return false;
    }

    virtual color emitted(
        const ray& r_in, const hit_record& hit_rec, double u, double v, const vec3& p
    ) const override{
        if (hit_rec.front_face)
            return emit->value(u, v, p);
        else return vec3(0, 0, 0);
    }

};

class isotropic: public material {
public:
    shared_ptr<texture> albedo;
public:
    isotropic(color _color): albedo(make_shared<solid_color>(_color)) {}
    isotropic(shared_ptr<texture> _albedo): albedo(_albedo) {}
    
    virtual bool scatter(
        const ray& r_in, const hit_record& hit_rec, scatter_record& srec
    ) const override {
        //r_out = ray(hit_rec.p, random_in_unit_sphere(), r_in.time());
        srec.is_specular = false;
        srec.pdf_ptr = nullptr;
        srec.attenuation = albedo->value(hit_rec.u, hit_rec.v, hit_rec.p);
        return true;
    }
};