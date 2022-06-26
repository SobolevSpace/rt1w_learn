#pragma once

#include "common.h"

class camera{
public:
    point3 origin;
    vec3 horizontal;
    vec3 vertical;
    point3 lower_left_corner;

public:
    camera( point3 lookfrom, 
    point3 lookat, 
    vec3 up, 
    double vfov, double aspect_ratio) {
        
        const double theta = degree2radians(vfov);
        const double h = tan(theta/2);
        const double viewport_height = 2.0 * h;
        const double viewport_width = viewport_height * aspect_ratio;
        const double focal_length = 1.0;
        vec3 w = unit_vector(lookfrom - lookat);
        vec3 u = unit_vector(cross(up, w));
        vec3 v = cross(w, u);
        origin = lookfrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - w - horizontal/2 - vertical/2;
    }

    ray get_ray(double u, double v) const {
        return ray(origin, lower_left_corner-origin + u*horizontal + v*vertical);
    }
};