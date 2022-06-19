#pragma once

#include "vec3.h"

class ray{
public:
    point3 orig;
    vec3 dir;

public:
    ray() {}
    ray(const point3& o, const vec3& d):orig(o), dir(d) {}

    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    point3 at(double t) const {
        return orig + t * dir;
    }
};