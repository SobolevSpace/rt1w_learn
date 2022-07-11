#pragma once

#include "common.h"

class onb{
public:
    vec3 axis[3];
public:
    onb() {}

    inline vec3 operator[](int i) const {return axis[i];}

    vec3 u() const { return axis[0]; }
    vec3 v() const { return axis[1]; }
    vec3 w() const { return axis[2]; }

    vec3 local(double a, double b, double c) const {
        return a*axis[0] + b*axis[1] + c*axis[2];
    }

    vec3 local(const vec3& p) const {
        return p.x()*axis[0] + p.y()*axis[1] + p.z()*axis[2];
    }

    void build_from_w(const vec3& u);
};

void onb::build_from_w(const vec3& u) {
    axis[2] = unit_vector(u);
    vec3 a = fabs(w().x())>0.9?vec3(0, 1, 0):vec3(1, 0, 0);
    axis[1] = unit_vector(cross(axis[2], a));
    axis[0] = cross(axis[2], axis[1]);
}