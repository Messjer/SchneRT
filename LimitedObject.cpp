//
// Created by Hong Man Hou on 19/1/2019.
//

#include "LimitedObject.h"

using namespace std;
using namespace stage;

namespace stage {
    std::istream &operator >>(std::istream &fin, Rectangle &s) {
        string str;
        while(fin >> str) {
            if (str == "end") break;
            if (str == "plane") fin >> s.plane;
            else if (str == "low") fin >>s.low;
            else if (str == "high") fin >>s.high;
            else if (str == "scaled") fin >>s.scaled;
            else if (str == "texture") {
                s.texture = new Canvas();
                fin >>str;
                s.texture->read_png(str);
            }
            else {
                cerr <<string("Unrecognized field: ") + str + " for Limited Plane!" <<endl;
                exit(-1);
            }
        }
        for (int d = 0; d < 3; d++)
            if(abs(s.plane.normalized[d]) < EPS) {
                s.width[d] = s.high[d] - s.low[d];
            }
        s.diff = s.plane.diff;
        s.refr = s.plane.refr;
        s.spec = s.plane.spec;
        s.color = s.plane.color;
        s.emit = s.plane.emit;
        return fin;
    }

    std::ostream &operator <<(std::ostream &fout, const Rectangle &s) {
        fout <<"Rectangle:" <<endl;
        fout <<"Underlying plane is "<<s.plane <<endl;
        fout <<"low is " <<s.low <<endl;
        fout <<"high is " <<s.high <<endl;
        fout <<"width is " <<s.width <<endl;
        return fout;
    }

    std::istream &operator >>(std::istream &fin, Disk &s) {
        string str;
        while(fin >> str) {
            if (str == "end") break;
            if (str == "plane") fin >> s.plane;
            else if (str == "pos") fin >>s.pos;
            else if (str == "rad") fin >>s.rad;
            else {
                cerr <<string("Unrecognized field: ") + str + " for Disk!" <<endl;
                exit(-1);
            }
        }
        s.diff = s.plane.diff;
        s.refr = s.plane.refr;
        s.spec = s.plane.spec;
        s.color = s.plane.color;
        s.emit = s.plane.emit;
        return fin;
    }

    std::ostream &operator <<(std::ostream &fout, const Disk &s) {
        fout <<"Disk:" <<endl;
        fout <<"Underlying plane is "<<s.plane <<endl;
        fout <<"center is " <<s.pos <<endl;
        fout <<"radius is " <<s.rad <<endl;
        return fout;
    }

    std::istream &operator >>(std::istream &fin, HoledDisk &s) {
        string str;
        Vec axis;
        double theta = 0;
        while(fin >> str) {
            if (str == "end") break;
            if (str == "plane") fin >> s.plane;
            else if (str == "pos") fin >>s.pos;
            else if (str == "rad_in") fin >>s.rad_in;
            else if (str == "rad_out") fin >>s.rad_out;
            else if (str == "axis") fin >>axis;
            else if (str == "theta") fin >>theta;
            else if (str == "rad_out") fin >>s.rad_out;
            else {
                cerr <<string("Unrecognized field: ") + str + " for Disk!" <<endl;
                exit(-1);
            }
        }
        if (theta >= EPS) {
            //cout <<s.plane.dist <<endl;
            s.plane.set(s.plane.dist, s.plane.normalized.rotate(axis, theta));
        }
        double yy = s.pos.dot(s.plane.normalized) + s.plane.dist;
        cout <<yy <<endl;
        s.pos = s.pos - s.plane.normalized * yy;
        s.diff = s.plane.diff;
        s.refr = s.plane.refr;
        s.spec = s.plane.spec;
        s.color = s.plane.color;
        s.emit = s.plane.emit;
        return fin;
    }


    std::ostream &operator <<(std::ostream &fout, const HoledDisk &s) {
        fout <<"Disk:" <<endl;
        fout <<"Underlying plane is "<<s.plane <<endl;
        fout <<"center is " <<s.pos <<endl;
        fout <<"rad_in is " <<s.rad_in <<endl;
        fout <<"rad_out is " <<s.rad_out <<endl;
        return fout;
    }
}

Intersection Rectangle::intersect(const Ray &ray) const {
    Intersection tmp = plane.intersect(ray);
    if (tmp.type == MISS || !contains(tmp.poc)) {
        tmp.type = MISS;
        return tmp;
    }
    tmp.hit = this;
    return tmp;
}

bool Rectangle::contains(const Vec &pt) const {
    bool valid = true;
    for (int d = 0; d < 3; d++) {
        if (pt[d] > high[d] + pos[d] + EPS || pt[d] < low[d] + pos[d] - EPS)
            valid = false;
    }
    return valid;
}

Vec Rectangle::get_color(const Vec &pt) const {
    // only axis-aligen textures are supported currently
    if (texture != nullptr) {
        for (int d = 0; d < 3; d++)
            if (abs(abs(plane.normalized[d]) - 1) < EPS) {
                if (scaled == 0) {
                    double x, y;
                    x = get_offset(pt, (d + 1) % 3);
                    y = get_offset(pt, (d + 2) % 3);
                    return texture->get_color(y, x);
                } else {
                    int x, y;
                    x = get_offset_unscaled(pt, (d + 1) % 3) * scaled;
                    y = get_offset_unscaled(pt, (d + 2) % 3) * scaled;
                    return texture->get_color(y, x);
                }
            } else {
                cerr <<"only x-y,y-z,z-x planes are currently supported" <<endl;
                exit(5);
            }
    } else
        return color;
}

double Rectangle::get_offset(const Vec &pt, int d) const {
    return (pt[d] - low[d]) / width[d];
}

double Rectangle::get_offset_unscaled(const Vec &pt, int d) const {
    return (pt[d] - low[d]);
}


Intersection Disk::intersect(const Ray &ray) const {
    Intersection tmp = plane.intersect(ray);
    if (tmp.type == MISS || !contains(tmp.poc)) {
        tmp.type = MISS;
        return tmp;
    }
    tmp.hit = this;
    return tmp;
}

bool Disk::contains(const Vec &pt) const {
    return (pt - pos).norm() <= rad;
}

Intersection HoledDisk::intersect(const Ray &ray) const {
    Intersection tmp = plane.intersect(ray);
    //return tmp;
    if (tmp.type == MISS || !contains(tmp.poc)) {
        tmp.type = MISS;
        return tmp;
    }
    tmp.hit = this;
    return tmp;
}

bool HoledDisk::contains(const Vec &pt) const {
    double dist = (pt - pos).norm();
    //return dist <= rad_out;
    return dist <= rad_out && dist >= rad_in;
}