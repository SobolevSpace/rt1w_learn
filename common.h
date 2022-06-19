#pragma once

#include <cmath>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::sqrt;
using std::make_shared;

const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

inline double degree2radians(double deg) {
    return deg*PI/180.0;
}

#include "ray.h"
#include "vec3.h"