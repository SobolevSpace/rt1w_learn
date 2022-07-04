#pragma once

#include "common.h"
#include "exlib/rtw_stb_image.h"
#include "perlin.h"

class texture{
public:
    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color: public texture {
private:
    color color_value;
public:
    solid_color() {}
    solid_color(const color& _color):color_value(_color) {}
    solid_color(double r, double g, double b):color_value(r, g, b) {}

    virtual color value(double u, double v, const point3& p) const override {
        return color_value;
    }

};

class checker_texture: public texture{
public:
    shared_ptr<texture> odd;
    shared_ptr<texture> even;
public:
    checker_texture() {}
    checker_texture(shared_ptr<texture> _odd, shared_ptr<texture> _even): odd(_odd), even(_even) {}
    checker_texture(const color& _odd, const color& _even):
        odd(make_shared<solid_color>(_odd)), even(make_shared<solid_color>(_even)) {}
    
    virtual color value(double u, double v, const point3& p) const override {
        double sines = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
        if (sines < 0) return odd->value(u, v, p);
        else return even->value(u, v, p);
    }
};

class noise_texture: public texture {
public:
    perlin noise;
    double scale;

    noise_texture(): noise() {}
    noise_texture(double _scale): scale(_scale) {}

    virtual color value(double u, double v, const point3& p) const override {
        return color(1, 1, 1)*0.5*(1+sin(scale*p.z() + 10*noise.disturb(p)));
    }
};

class image_texture: public texture {
private:
    unsigned char* data;
    int width, height;
    int byte_per_row;
public:
    const static int byte_per_pixel = 3;
    image_texture(): data(nullptr), width(0), height(0), byte_per_row(0) {} 

    image_texture(const char* filename) {
        int temp_byte_per_pixel = byte_per_pixel;
        data = stbi_load(filename, &width, &height, &temp_byte_per_pixel, byte_per_pixel);
        if (!data) {
            std::cerr<<"Could not load image texture file: "<<filename<<'\n';
            width = height = 0;
        }
        byte_per_row = byte_per_pixel * width;
    }

    ~image_texture() {
        delete data;
    }

    virtual color value(double u, double v, const point3& p) const override {
        if (data == nullptr) {
            return color(0, 1, 1);
        }
        u = clamp(u, 0, 1);
        v = 1.0 - clamp(v, 0, 1);
        int i = static_cast<int>(u*width), j = static_cast<int>(v*height);
        if (i>=width) i = width-1;
        if (j>=height) j = height-1;
        double color_scale = 1.0/255;
        const unsigned char* pix_ptr = data + j*byte_per_row + i*byte_per_pixel;
        return color_scale*color(pix_ptr[0], pix_ptr[1], pix_ptr[2]);
    }
};