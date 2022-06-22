#pragma once

#include "common.h"
#include <iostream>

void write_color(std::ostream& out, color pixel_color, int spp) {
    double r = pixel_color.x(), g = pixel_color.y(), b = pixel_color.z();
    //double scale = 1.0/spp;
    r /= spp;
    g /= spp;
    b /= spp;
    r = sqrt(r);
    g = sqrt(g);
    b = sqrt(b);
    out<<static_cast<int>(clamp(256*r, 0, 255)) <<' '
    <<static_cast<int>(clamp(256*g, 0, 255)) << ' '
    <<static_cast<int>(clamp(256*b, 0, 255))<<'\n';
}