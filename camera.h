#pragma once

#include "common.h"

class camera{
public:
    point3 origin;
    vec3 horizontal;
    vec3 vertical;
    point3 lower_left_corner;
    vec3 w, u, v;
    double lens_radius;

public:
    camera( point3 lookfrom, 
    point3 lookat, 
    vec3 up, 
    double vfov, double aspect_ratio,
    double aperture, double focus_dist) {
        
        const double theta = degree2radians(vfov);
        const double h = tan(theta/2);
        const double viewport_height = 2.0 * h;
        const double viewport_width = viewport_height * aspect_ratio;
        const double focal_length = 1.0;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(up, w));
        v = cross(w, u);
        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - focus_dist * w - horizontal/2 - vertical/2;
    
        lens_radius = aperture/2;
    }

    ray get_ray(double s, double t) const {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u*rd.x() + v*rd.y();
        return ray(origin + offset, 
        lower_left_corner-origin + s*horizontal + t*vertical - offset);
    }
};