#pragma once

#include "vec3.h"

class ray{
public:
    point3 orig;
    vec3 dir;
    double tm;

public:
    ray() {}
    ray(const point3& o, const vec3& d, double time):
        orig(o), dir(d), tm(time) {}

    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }
    double time() const {return tm; }

    point3 at(double t) const {
        return orig + t * dir;
    }
};