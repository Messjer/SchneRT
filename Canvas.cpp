//
// Created by Hong Man Hou on 25/11/2018.
//

#include "Canvas.h"
#include "png++/image.hpp"

void Canvas::draw_to_file(const std::string &file) {
    png::image< png::rgb_pixel > img(w,h);
    for (size_t y = 0; y < img.get_height(); ++y)
    {
        for (size_t x = 0; x < img.get_width(); ++x)
        {
            auto &color = colors[y * w + x];
            img[y][x] = png::rgb_pixel(d_to_255(color.x), d_to_255(color.y), d_to_255(color.z));
            // non-checking equivalent of image.set_pixel(x, y, ...);
        }
    }
    img.write(file);
}