//
// Created by Hong Man Hou on 25/11/2018.
//

#include "Object.h"

using namespace stage;
using namespace std;

namespace stage {
    ostream &operator<<(std::ostream &fout, const Object &o) {
        switch (o.get_type()) {
            case Object::SPHERE:
                fout << (*((Sphere *) &o)) << endl;
        }
        return fout;
    }

    ostream &operator<<(std::ostream &fout, const Sphere &s) {
        fout << "Sphere of radius : " <<s.rad << endl;
        fout << "At : " << s.pos <<" , ";
        fout << "Color : " << s.color << " , ";
        fout << "Emit : " << s.emit << " , ";
        fout << "Refr : " << s.refr << " , ";
        fout << "Spec : " << s.spec << " , ";
        fout << "Diff : " << s.diff;
        return fout;
    }
    std::istream &operator >>(std::istream &fin, Sphere &s) {
        string str;
        while(fin >> str) {
            if (str == "end") break;
            if (str == "pos") fin >> s.pos;
            else if (str == "rad") fin >> s.rad;
            else if (str == "diff") fin >> s.diff;
            else if (str == "spec") fin >> s.spec;
            else if (str == "refr") fin >> s.refr;
            else if (str == "color") fin >> s.color;
            else if (str == "emit") fin >> s.emit;
            else {
                cerr <<string("Unrecognized field: ") + str + " for sphere!" <<endl;
                exit(-1);
            }
        }
        return fin;
    }
}


Intersection Sphere::intersect(const stage::Ray &ray) const {
    Intersection rst;
    // Solve (td + src - pos)^2 = R^2
    // i.e. t^2 d.d + 2t(src - pos).d + (src - pos).(src - pos) = R^2
    // By . we mean dot product
    Vec sp = pos - ray.src;
    double b = sp.dot(ray.dir);
    double t = 0, delta = b * b - sp.dot(sp) + rad * rad;

    // no solution
    if (delta < EPS) return rst;
    else delta = sqrt(delta);


    if ((t = b - delta) > EPS) {
        rst.t = t;
        rst.type = INTO;
    }
    else if ((t = b + delta) > EPS) {
        rst.t = t;
        rst.type = OUTO;
    } else
        return rst;

    rst.poc = ray.src + ray.dir * t;
    rst.normal = normal(rst.poc);
    rst.hit = this;
    return rst;
}

Vec Sphere::normal(const Vec &pt) const {
    // assert(touched(ray.src));
    return (pt - pos).unit();
}

