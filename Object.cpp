//
// Created by Hong Man Hou on 25/11/2018.
//

#include "Object.h"

using namespace stage;
using namespace std;

namespace stage {
    ostream &operator<<(std::ostream &fout, Object &o) {
        switch (o.get_type()) {
            case Object::SPHERE:
                fout << (*((Sphere *) &o)) << endl;
        }
        return fout;
    }

    ostream &operator<<(std::ostream &fout, Sphere &s) {
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
                throw string("No field") + str + "exists for sphere!\n";
            }
        }
        return fin;
    }
}


double Sphere::intersect(const stage::Ray &ray) const {
    // Solve (td + src - pos)^2 = R^2
    // i.e. t^2 d.d + 2t(src - pos).d + (src - pos).(src - pos) = R^2
    // By . we mean dot product
    Vec3d sp = pos - ray.src;
    double b = sp.dot(ray.dir);
    // cout <<ray.dir <<endl;
    double t = 0, delta = b * b - sp.dot(sp) + rad * rad;
    //cout <<delta <<endl;

    // no solution
    if (delta < EPS) return INF_D;
    else delta = sqrt(delta);


    if ((t = b - delta) > EPS)
        return t;
    else if ((t = b + delta) > EPS)
        return t;
    else
        return INF_D;
}

Vec3d Sphere::normal(const stage::Ray &ray) const {
    assert(touched(ray.src));
    return (ray.src - pos).unit();
}

