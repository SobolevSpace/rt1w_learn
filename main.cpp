#include "common.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "camera.h"
#include "aarect.h"

#include "material.h"

#include <iostream>

hittable_list two_spheres() {
    hittable_list world;
    shared_ptr<texture> checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));
    
    return world;
}

hittable_list random_scene() {
    hittable_list world;

    shared_ptr<texture> checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    vec3 center2 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<moving_sphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

hittable_list two_perlin_spheres() {
    hittable_list objects;
    shared_ptr<texture> pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));
    return objects;
}

hittable_list earth() {
    hittable_list objects;
    shared_ptr<texture> earthtex = make_shared<image_texture>("./resource/earthmap.jpg");
    shared_ptr<material> earthmat = make_shared<lambertian>(earthtex);
    objects.add(make_shared<sphere>(point3(0, 0, 0), 2, earthmat));
    return objects;
}

hittable_list simple_light() {
    hittable_list objects;
    shared_ptr<texture> pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));
    
    shared_ptr<material> difflight = make_shared<diffuse_light>(color(4, 4, 4));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));
    return objects;
}

color ray_color(const ray& r, const color& bg, const hittable& world, int depth) {
    hit_record hit_rec;
    if (depth <= 0) return color(0, 0, 0);

    if (!world.hit(r, 0.001, INF, hit_rec)) return bg;

    ray scattered;
    color attenuation;
    color emitted = hit_rec.mat_ptr->emitted(hit_rec.u, hit_rec.v, hit_rec.p);

    if (!hit_rec.mat_ptr->scatter(r, hit_rec, attenuation, scattered)) {
        return emitted;
    }
    return emitted + attenuation * ray_color(scattered, bg, world, depth-1);
}

int main(int, char**) {
    const double aspect_ratio = 16.0/9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    int spp = 100;
    const int max_depth = 50;
    
    point3 lookfrom;
    point3 lookat;
    double vfov = 40.0;
    double aperture = 0.0;
    color background(0, 0, 0);
    hittable_list world;

    switch(0) {
        case 1: {
            world = random_scene();
            background = color(0.7, 0.8, 1);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            aperture = 0.1;
            break;
        }
        
        case 2: {
            world = two_spheres();
            background = color(0.7, 0.8, 1);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            break;
        }
        
        case 3: {
            world = two_perlin_spheres();
            background = color(0.7, 0.8, 1);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            break;
        }

        case 4: {
            world = earth();
            background = color(0.7, 0.8, 1);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            break;
        }

        default:
        case 5: {
            world = simple_light();
            spp = 400;
            background = color(0, 0, 0);
            lookfrom = point3(26, 3, 6);
            lookat = point3(0, 2, 0);
            vfov = 20.0;
            break;
        }

        case 6: {

        }
    }

    vec3 vup(0, 1, 0);
    double dist_to_focus = 10.0;
    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    
    

    //auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    //auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    //auto material_left   = make_shared<dielectric>(1.5);
    //auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    //world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    //world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    //world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    //world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0), -0.45, material_left));
    //world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    
    std::cout<<"P3\n"<<image_width<< ' '<<image_height<<"\n255\n";

    for (int j=image_height-1;j>=0;j--) {
        std::cerr << "\rScanlines remaining: "<<j<<' '<<std::flush;
        for (int i=0;i<image_width;i++) {
            color pixel_color(0, 0, 0);
            for (int _=0;_<spp;_++) {
                double u = (i + random_double())/(image_width-1);
                double v = (j + random_double())/(image_height-1);
                pixel_color += ray_color(cam.get_ray(u, v), background, world, max_depth);
            }
            write_color(std::cout, pixel_color, spp);
        }
    }
    std::cerr<<"\nDone.\n";
    return 0;
}
