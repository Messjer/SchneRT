//
// Created by Hong Man Hou on 25/11/2018.
//

#include "Canvas.h"
#include "png++/image.hpp"

void Canvas::write_ppm_head(FILE *file) {
    fprintf(file, "P3\n%d %d\n%d\n", w, h, 255);
}

void Canvas::draw_to_file(const std::string &file) {
    png::image< png::rgb_pixel > img(w,h);
    for (size_t y = 0; y < img.get_height(); ++y)
    {
        for (size_t x = 0; x < img.get_width(); ++x)
        {
            auto &color = colors[y * w + x];
            //img[y][x] = png::rgb_pixel(d_to_255(color.x), d_to_255(color.y), d_to_255(color.z));
            img.set_pixel(x, y, png::rgb_pixel(d_to_255(color.x), d_to_255(color.y), d_to_255(color.z)));
            // non-checking equivalent of image.set_pixel(x, y, ...);
        }
    }
    img.write(file);
}