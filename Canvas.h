//
// Created by Hong Man Hou on 25/11/2018.
//

#ifndef SCHNERT_CANVAS_H
#define SCHNERT_CANVAS_H
#include "utils.h"

// A class representing the result image
class Canvas {
private:
    int w, h;
    Vec3d *colors;
    void write_ppm_head(FILE *file);
    bool in_range(int y, int x) { return y >= 0 && x >= 0 & y < h && x < w; }
    int coord(int y, int x) { assert(in_range(y, x) && "Not In range!"); return y * w + x; }
    long int d_to_255(double d) { d = (d < 0 ? 0 : (d > 1 ? 1 : d)); return int(pow(d, 1/2.2) * 255 + .5); }
public:
    Canvas(int h, int w): w(w), h(h) {
        colors = new Vec3d[w * h];
    }
    ~Canvas() {
        delete colors;
    }
    void add_paint(int y, int x, const Vec3d &color) {
        Vec3d & prev = colors[coord(y, x)];
        prev = prev + color;
    }
    void draw(int y, int x, const Vec3d &color) {
        //std::cout <<coord(y, x);
        colors[coord(y, x)] = color;
    }
    void draw_to_file(const std::string &file);
};


#endif //SCHNERT_CANVAS_H
