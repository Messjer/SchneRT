//
// Created by Hong Man Hou on 25/11/2018.
//

#ifndef SCHNERT_UTILS_H
#define SCHNERT_UTILS_H

#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <list>
#include <cassert>

struct Vec3d {
    double x, y, z;
    Vec3d(double x = 0, double y = 0, double z = 0): x(x), y(y), z(z) {};
    Vec3d operator+(const Vec3d &other) const { return {x+other.x,y+other.y,z+other.z}; }
    Vec3d operator-(const Vec3d &other) const { return {x-other.x,y-other.y,z-other.z}; }
    Vec3d operator*(double scale) const { return {x*scale,y*scale,z*scale}; }
    Vec3d operator*(const Vec3d &other) const { return {x*other.x,y*other.y,z*other.z}; }
    Vec3d& norm(){ return *this = *this * (1/sqrt(x*x+y*y+z*z)); }
    double dot(const Vec3d &other) const { return x*other.x+y*other.y+z*other.z; }
    // cross product
    Vec3d cross(Vec3d &other) const {return {y*other.z-z*other.y,z*other.x-x*other.z,x*other.y-y*other.x};}

    friend std::ostream &operator <<(std::ostream &out, const Vec3d &v) {
        out <<v.x <<" , " <<v.y << " , " <<v.z;
        return out;
    }
};

namespace stage {
    const double EPS = 1e-4;
    const double INF_D = 1e20;
}

#endif //SCHNERT_UTILS_H
