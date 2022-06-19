#include "common.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>


color ray_color(const ray& r, const hittable& world) {
    hit_record hit_rec;
    if (world.hit(r, 0, INF, hit_rec)) {
        return 0.5*(hit_rec.normal+color(1, 1, 1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    double t = (unit_direction.y() + 1)*0.5;
    return (1-t)*vec3(1, 1, 1) + t*vec3(0.5, 0.7, 1);
}

int main(int, char**) {
    const double aspect_ratio = 16.0/9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width/aspect_ratio);

    double viewport_height = 2.0;
    double viewport_width = viewport_height * aspect_ratio;
    double focal_length = 1.0;

    point3 origin = point3(0, 0, 0);
    point3 horizontal = point3(viewport_width, 0, 0);
    point3 vertical = point3(0, viewport_height, 0);
    point3 lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));
    
    std::cout<<"P3\n"<<image_width<< ' '<<image_height<<"\n255\n";

    for (int j=image_height-1;j>=0;j--) {
        std::cerr << "\rScanlines remaining: "<<j<<' '<<std::flush;
        for (int i=0;i<image_width;i++) {
            double u = double(i)/(image_width-1);
            double v = double(j)/(image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }
    std::cerr<<"\nDone.\n";
    return 0;
}
