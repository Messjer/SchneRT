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
    void writePPMHead(FILE * file);
    bool inRange(int y, int x) { return y >= 0 && x >= 0 & y < h && x < w; }
    int coord(int y, int x) { assert(inRange(y, x) && "Not In range!"); return y * w + x; }
public:
    Canvas(int h, int w): w(w), h(h) {
        colors = new Vec3d[w * h];
    }
    ~Canvas() {
        delete colors;
    }
    void draw(int y, int x, const Vec3d &color) {
        //std::cout <<coord(y, x);
        colors[coord(y, x)] = color;
    }
    void drawToFile(const std::string &file);
};


#endif //SCHNERT_CANVAS_H
