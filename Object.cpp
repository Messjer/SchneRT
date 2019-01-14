//
// Created by Hong Man Hou on 25/11/2018.
//

#include "Object.h"
#include "Bezier.h"

using namespace stage;
using namespace std;

namespace stage {
    ostream &operator<<(std::ostream &fout, const Object &o) {
        switch (o.get_type()) {
            case Object::SPHERE:
                fout << (*((Sphere *) &o)) << endl;
                break;
            case Object::BEZIER:
                fout << (*((BezierRotational *) &o)) << endl;
                break;
            case Object::PLANE:
                fout << (*((Plane *) &o)) << endl;
                break;
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
            else if (str == "absorb") fin >>s.absorb;
            else if (str == "n") fin >>s.n;
            else {
                cerr <<string("Unrecognized field: ") + str + " for sphere!" <<endl;
                exit(-1);
            }
        }
        return fin;
    }

    std::ostream &operator<<(std::ostream &fout, const Plane &s) {
        fout << "Plane At : " << s.pos <<" , ";
        fout << "Color : " << s.color << " , ";
        fout << "Emit : " << s.emit << " , ";
        fout << "Refr : " << s.refr << " , ";
        fout << "Spec : " << s.spec << " , ";
        fout << "Diff : " << s.diff;
        return fout;
    }

    std::istream &operator >>(std::istream &fin, Plane &s) {
        string str;
        while(fin >> str) {
            if (str == "end") break;
            if (str == "pos") fin >> s.pos;
            else if (str == "N") fin >> s.N;
            else if (str == "diff") fin >> s.diff;
            else if (str == "spec") fin >> s.spec;
            else if (str == "refr") fin >> s.refr;
            else if (str == "color") fin >> s.color;
            else if (str == "emit") fin >> s.emit;
            else {
                cerr <<string("Unrecognized field: ") + str + " for plane!" <<endl;
                exit(-1);
            }
        }
        s.dist = s.pos.norm();
        s.normalized = s.N;
        s.normalized.unit();
        return fin;
    }

    std::istream &operator >>(std::istream &fin, AABBox &s) {
        string str;
        while(fin >> str) {
            if (str == "end") break;
            if (str == "pos") fin >> s.pos;
            else if (str == "faces") {
                for (int i = 0; i < 6; i++) {
                    fin >> s.faces[i];
                    for (int d = 0; d < 3; d++) {
                        if (s.low[d] > s.faces[i].pos[d])
                            s.low[d] = s.faces[i].pos[d];
                        if (s.high[d] < s.faces[i].pos[d])
                            s.high[d] = s.faces[i].pos[d];
                    }
                }
            }
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

    std::ostream &operator <<(std::ostream &fout, const AABBox &s) {
        fout << "AABB At : " << s.pos <<" , ";
        cout << "Faces are " <<endl;
        for (int i = 0; i < 6; i++)
            fout <<s.faces[i] <<endl;
        fout << "Color : " << s.color << " , ";
        fout << "Emit : " << s.emit << " , ";
        fout << "Refr : " << s.refr << " , ";
        fout << "Spec : " << s.spec << " , ";
        fout << "Diff : " << s.diff;
        return fout;
    };
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
    rst.normal = normal(rst.poc) * (rst.type == INTO? 1 : -1);
    rst.hit = this;
    return rst;
}

Vec Sphere::normal(const Vec &pt) const {
    // assert(touched(ray.src));
    return (pt - pos).unit();
}

Intersection Plane::intersect(const Ray &ray) const {
    Intersection rst;
    double dot = normalized.dot(ray.dir);
    if(fabs(dot) < EPS) return rst;
    double t = -(dist + normalized.dot(ray.src)) / dot;
    if (t > EPS)
    {
        double sgn = dot > EPS ? -1 : 1;
        rst.t = t;
        rst.type = OUTO;
        rst.poc = ray.src + ray.dir * t;
        rst.normal = normalized * sgn;
        rst.hit = this;
        return rst;
    } else
        return rst;
}

Intersection AABBox::intersect(const Ray &ray) const {
    Intersection rst;
    for (int i = 0; i < 6; i++) {
        Intersection tmp = faces[i].intersect(ray);
        if (tmp.type != MISS && tmp.t < rst.t) {
            Vec poc = tmp.poc;
            bool valid = true;
            for (int d = 0; d < 3; d++) {
                if (poc[d] > high[d] || poc[d] < low[d])
                    valid = false;
            }
            if (valid) {
                rst = tmp;
                rst.hit = this;
            }
        }
    }
    return rst;
}