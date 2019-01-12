//
// Created by Hong Man Hou on 25/11/2018.
//

#include "Canvas.h"

void Canvas::write_ppm_head(FILE *file) {
    fprintf(file, "P3\n%d %d\n%d\n", w, h, 255);
}

void Canvas::draw_to_file(const std::string &file) {
    FILE * out = fopen(file.c_str(), "w");
    if (out == nullptr) {
        fprintf(stderr, "Error opening file : %s", file.c_str());
        return;
    }
    write_ppm_head(out);
    for (int i = 0; i < w * h; i++) {
        auto &color = colors[i];
        fprintf(out, " %ld %ld %ld", d_to_255(color.x), d_to_255(color.y), d_to_255(color.z));
    }
    fclose(out);
}