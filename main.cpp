#include "common.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

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
    const int spp = 100;
    camera cam;

    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));
    
    std::cout<<"P3\n"<<image_width<< ' '<<image_height<<"\n255\n";

    for (int j=image_height-1;j>=0;j--) {
        std::cerr << "\rScanlines remaining: "<<j<<' '<<std::flush;
        for (int i=0;i<image_width;i++) {
            color pixel_color(0, 0, 0);
            for (int _=0;_<spp;_++) {
                double u = (i + random_double())/(image_width-1);
                double v = (j + random_double())/(image_height-1);
                pixel_color += ray_color(cam.get_ray(u, v), world);
            }
            write_color(std::cout, pixel_color, spp);
        }
    }
    std::cerr<<"\nDone.\n";
    return 0;
}
