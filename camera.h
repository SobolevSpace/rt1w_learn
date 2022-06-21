#pragma once

#include "common.h"

class camera{
public:
    point3 origin;
    vec3 horizontal;
    vec3 vertical;
    point3 lower_left_corner;

public:
    camera() {
        const double aspect_ratio = 16.0/9.0;
        
        const double viewport_height = 2.0;
        const double viewport_width = viewport_height * aspect_ratio;
        const double focal_length = 1.0;

        origin = point3(0, 0, 0);
        horizontal = vec3(viewport_width, 0, 0);
        vertical = vec3(0, viewport_height, 0);
        lower_left_corner = origin - vec3(0, 0, focal_length) - horizontal/2 - vertical/2;
    }

    ray get_ray(double u, double v) const {
        return ray(origin, lower_left_corner-origin + u*horizontal + v*vertical);
    }
};