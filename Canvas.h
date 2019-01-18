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
    Vec *colors;
    bool in_range(int y, int x) { return y >= 0 && x >= 0 & y < h && x < w; }
    int coord(int y, int x) { assert(in_range(y, x) && "Not In range!"); return y * w + x; }
    long int d_to_byte(double d) { d = clamp(d); return int(pow(d, 1/2.2) * 255 + .5); }
    double byte_to_d(int b) { double x = clamp(b,0,255); return pow((x / 255), 2.2); }
public:
    Canvas() {}
    Canvas(int h, int w): w(w), h(h) {
        colors = new Vec[w * h];
    }
    ~Canvas() {
        delete[] colors;
    }
    void read_png(std::string fname);
    void add_paint(int y, int x, const Vec &color) {
        Vec & prev = colors[coord(y, x)];
        prev = prev + color;
    }
    void draw(int y, int x, const Vec &color) {
        colors[coord(y, x)] = color;
    }
    Vec get_color(int y, int x) { return colors[y * w + x]; }
    Vec get_color(double y, double x) { int yy = clamp(y) * h; int xx = clamp(x) * w; return get_color(yy, xx); }
    void draw_to_file(const std::string &file);
    double clamp(double x, double l, double h) { return (x > h? h : (x < l ? l : x)); }
    double clamp(double x) { return clamp(x,0,1); }
};


#endif //SCHNERT_CANVAS_H
