//
// Created by Hong Man Hou on 25/11/2018.
//

#include "Canvas.h"
#include "png++/image.hpp"
#include "png++/require_color_space.hpp"

void Canvas::draw_to_file(const std::string &file) {
    //std::cout <<"JDLKS" <<std::endl;
    png::image< png::rgb_pixel > img(w,h);
    for (size_t y = 0; y < img.get_height(); ++y)
    {
        for (size_t x = 0; x < img.get_width(); ++x)
        {
            auto &color = colors[y * w + x];
            img[y][x] = png::rgb_pixel(d_to_byte(color.x), d_to_byte(color.y), d_to_byte(color.z));
            // non-checking equivalent of image.set_pixel(x, y, ...);
        }
    }
    img.write(file);
}

void Canvas::read_png(std::string fname) {
    png::image< png::rgb_pixel > img(fname.c_str(), png::require_color_space< png::rgb_pixel >());
    w = img.get_width();
    h = img.get_height();
    colors = new Vec[w * h];
    for (size_t y = 0; y < h; ++y)
    {
        for (size_t x = 0;  x < w; ++x)
        {
            auto rgb = img.get_pixel(x, y);
            colors[y * w + x] = Vec(byte_to_d(rgb.red), byte_to_d(rgb.green), byte_to_d(rgb.blue));
        }
    }
}