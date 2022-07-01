#pragma once
#include "common.h"

class aabb{
public:
    point3 minimum;
    point3 maximum;

public:
    aabb() {}
    aabb(const point3& _min, const point3& _max): minimum(_min), maximum(_max) {}

    point3 min() const { return minimum; }
    point3 max() const { return maximum; }

    bool hit(const ray& r, double t_min, double t_max) const {
        for (int i=0;i<3;i++) {
            double invD = 1.0/r.direction()[i];
            double t0 = (minimum[i]-r.origin()[i])*invD;
            double t1 = (maximum[i]-r.origin()[i])*invD;
            if (invD < 0) {
                std::swap(t0, t1);
            }
            t_min = t0>t_min?t0:t_min;
            t_max = t1<t_max?t1:t_max;
            if (t_max <= t_min) return false;
        }
        return true;
    }
};

aabb surrounding_box(const aabb& a, const aabb& b) {
    point3 mn(
        fmin(a.minimum[0], b.minimum[0]),
        fmin(a.minimum[1], b.minimum[1]),
        fmin(a.minimum[2], b.minimum[2]));
    point3 mx(
        fmax(a.maximum[0], b.maximum[0]),
        fmax(a.maximum[1], b.maximum[1]),
        fmax(a.maximum[2], b.maximum[2]));
    return aabb(mn, mx);
}