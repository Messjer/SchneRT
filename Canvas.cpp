//
// Created by Hong Man Hou on 25/11/2018.
//

#include "Canvas.h"

void Canvas::writePPMHead(FILE *file) {
    fprintf(file, "P3\n%d %d\n%d\n", w, h, 255);
}

void Canvas::drawToFile(const std::string &file) {
    FILE * out = fopen(file.c_str(), "w");
    if (out == nullptr) {
        fprintf(stderr, "Error opening file : %s", file.c_str());
        return;
    }
    writePPMHead(out);
    for (int i = 0; i < w * h; i++) {
        auto &color = colors[i];
        fprintf(out, " %ld %ld %ld", lround(color.x), lround(color.y), lround(color.z));
    }
    fclose(out);
}