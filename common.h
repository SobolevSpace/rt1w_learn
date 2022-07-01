#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>

using std::shared_ptr;
using std::sqrt;
using std::make_shared;

const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

inline double degree2radians(double deg) {
    return deg*PI/180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double t_min, double t_max) {
    return t_min + random_double()*(t_max-t_min);
}

inline int random_int(int t_min, int t_max) {
    return static_cast<int>(random_double(t_min, t_max+1));
}

inline double clamp(double t, double t_min, double t_max) {
    return t<t_min?t_min:(t>t_max?t_max:t);
}

#include "ray.h"
#include "vec3.h"