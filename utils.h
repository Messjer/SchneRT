//
// Created by Hong Man Hou on 25/11/2018.
//

#ifndef SCHNERT_UTILS_H
#define SCHNERT_UTILS_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <list>
#include <cassert>

typedef unsigned int uint;

namespace stage {
    static const double EPS = 1e-6;
    static const double INF_D = 1e20;
    static const double PI = acos(-1);
}

struct Vec {
    double x, y, z;
    Vec(double x = 0, double y = 0, double z = 0): x(x), y(y), z(z) {};
    Vec operator+(const Vec &other) const { return {x+other.x,y+other.y,z+other.z}; }
    Vec operator-(const Vec &other) const { return {x-other.x,y-other.y,z-other.z}; }
    Vec operator*(double scale) const { return {x*scale,y*scale,z*scale}; }
    Vec operator*(const Vec &other) const { return {x*other.x,y*other.y,z*other.z}; }
    Vec& unit(){ return *this = *this * (1/norm()); }
    double norm() const { return sqrt(x*x+y*y+z*z); }
    double inf_norm() const { return fmax(fmax(abs(x),abs(y)),abs(z));}
    double dot(const Vec &other) const { return x*other.x+y*other.y+z*other.z; }
    // cross product
    Vec cross(Vec &other) const {return {y*other.z-z*other.y,z*other.x-x*other.z,x*other.y-y*other.x};}
    Vec rotate(Vec axis, double theta) const {
        // the following function implements a 3d rotation
        // referenced Raina's implementation
        Vec ret;
        double cost = cos( theta );
        double sint = sin( theta );
        ret.x += x * ( axis.x * axis.x + ( 1 - axis.x * axis.x ) * cost );
        ret.x += y * ( axis.x * axis.y * ( 1 - cost ) - axis.z * sint );
        ret.x += z * ( axis.x * axis.z * ( 1 - cost ) + axis.y * sint );
        ret.y += x * ( axis.y * axis.x * ( 1 - cost ) + axis.z * sint );
        ret.y += y * ( axis.y * axis.y + ( 1 - axis.y * axis.y ) * cost );
        ret.y += z * ( axis.y * axis.z * ( 1 - cost ) - axis.x * sint );
        ret.z += x * ( axis.z * axis.x * ( 1 - cost ) - axis.y * sint );
        ret.z += y * ( axis.z * axis.y * ( 1 - cost ) + axis.x * sint );
        ret.z += z * ( axis.z * axis.z + ( 1 - axis.z * axis.z ) * cost );
        return ret;
    }
    double &operator [] (int i) {
        switch (i) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                std::cerr <<"vector index out of bounds!" <<std::endl;
                exit(-1);
        }
    }

    const double &operator [] (int i) const {
        switch (i) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                std::cerr <<"vector index out of bounds!" <<std::endl;
                exit(-1);
        }
    }

    //Vec get_exp() {return {exp(x), exp(y), exp(z)}; }

    friend std::ostream &operator <<(std::ostream &out, const Vec &v) {
        out <<v.x <<" , " <<v.y << " , " <<v.z;
        return out;
    }
    friend std::istream &operator >>(std::istream &fin, Vec &s) {
        fin >>s.x >>s.y >>s.z;
        return fin;
    }
};
#endif //SCHNERT_UTILS_H
