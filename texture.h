#pragma once

#include "common.h"
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
        return color(1, 1, 1)*noise.noise(scale*p);
    }
};