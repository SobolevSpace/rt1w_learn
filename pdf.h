#pragma once

#include "common.h"
#include "onb.h"

class pdf{
public:
    virtual ~pdf() {}

    virtual double value(const vec3& dir) const = 0;
    virtual vec3 generate() const = 0;
};

class cosine_pdf: public pdf{
public:
    onb uvw;
public:
    cosine_pdf(const vec3& n): uvw() {
        uvw.build_from_w(n);
    }

    virtual double value(const vec3& dir) const override {
        double cosine = dot(unit_vector(dir), uvw.w());
        return cosine > 0? cosine/PI: 0;
    }

    virtual vec3 generate() const override{
        return uvw.local(random_cosine_direction());
    }
};

class hittable_pdf: public pdf{
public:
    shared_ptr<hittable> obj_ptr;
    point3 p;
public:
    hittable_pdf(shared_ptr<hittable> _obj_ptr, point3 _p):obj_ptr(_obj_ptr), p(_p) {}

    virtual double value(const vec3& dir) const override {
        return obj_ptr->pdf_value(p, dir);
    }

    virtual vec3 generate() const override {
        return obj_ptr->random(p);
    }
};

class mixture_pdf: public pdf {
public:
    shared_ptr<pdf> p[2];
public:
    mixture_pdf(shared_ptr<pdf> p1, shared_ptr<pdf> p2) {
        p[0] = p1;
        p[1] = p2;
    }

    virtual double value(const vec3& dir) const override {
        return 0.5*p[0]->value(dir) + 0.5*p[1]->value(dir);
    }

    virtual vec3 generate() const override {
        if (random_double() < 0.5) return p[0]->generate();
        return p[1]->generate();
    }
};