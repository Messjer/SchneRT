//
// Created by Hong Man Hou on 19/1/2019.
//

#include "Plane.h"

using namespace std;
using namespace stage;

namespace stage {
    std::ostream &operator<<(std::ostream &fout, const Plane &s) {
        fout << "Plane At : " << s.pos <<" , ";
        fout << "Color : " << s.color << " , ";
        fout << "Emit : " << s.emit << " , ";
        fout << "Refr : " << s.refr << " , ";
        fout << "Spec : " << s.spec << " , ";
        fout << "Dist : " << s.dist << " , ";
        fout << "Diff : " << s.diff;
        return fout;
    }

    std::istream &operator >>(std::istream &fin, Plane &s) {
        string str;
        while(fin >> str) {
            if (str == "end") break;
            if (str == "dist") fin >> s.dist;
            else if (str == "N") fin >> s.normalized;
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
        s.set(s.dist, s.normalized.unit());
        return fin;
    }

    std::istream &operator >>(std::istream &fin, AABBox &s) {
        double scale = 1;
        string str;
        while(fin >> str) {
            if (str == "end") break;
            if (str == "pos") fin >> s.pos;
            else if (str == "faces") {
                for (int i = 0; i < 6; i++) {
                    fin >> s.faces[i];
                    int d = i / 2;
                    if (i & 1)
                        s.low[d] = s.faces[i].pos[d];
                    else
                        s.high[d] = s.faces[i].pos[d];
                }
            } else if (str == "low") fin >>s.low;
            else if (str == "high") fin >>s.high;
            else if (str == "n") fin >>s.n;
            else if (str == "diff") fin >> s.diff;
            else if (str == "spec") fin >> s.spec;
            else if (str == "refr") fin >> s.refr;
            else if (str == "absorb") fin >> s.absorb;
            else if (str == "color") fin >> s.color;
            else if (str == "scale") fin >> scale;
            else if (str == "emit") fin >> s.emit;
            else {
                cerr <<string("Unrecognized field: ") + str + " for AABB!" <<endl;
                exit(-1);
            }
        }
        // if scale is 0, then user should enter faces
        // otherwise we require used to enter low & high
        if (scale != 1) {
            s.make_faces();
            s.transform(s.pos, scale);
        }
        return fin;
    }

    std::ostream &operator <<(std::ostream &fout, const AABBox &s) {
        fout << "AABB At : " << s.pos <<endl;
        fout << "low is " <<s.low <<" high is " <<s.high <<endl;
        fout << "Faces are " <<endl;
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

Intersection Plane::intersect(const Ray &ray, bool with_BB) const {
    Intersection rst;
    double dot = normalized.dot(ray.dir);
    if(fabs(dot) < EPS) return rst;
    double t = -(dist + normalized.dot(ray.src)) / dot;
    if (t > 2 * EPS)
    {
        double sgn = dot > EPS ? -1 : 1;
        rst.t = t;
        rst.type = INTO;
        rst.poc = ray.src + ray.dir * t;
        rst.normal = normalized * sgn;
        rst.hit = this;
        return rst;
    } else
        return rst;
}

Intersection AABBox::intersect(const Ray &ray, bool with_BB) const {
    Intersection rst;
    HitType type =  contains(ray.src) ? OUTO : INTO;
    for (int i = 0; i < 6; i++) {
        Intersection tmp = faces[i].intersect(ray, false);
        if (tmp.type != MISS && tmp.t > EPS && tmp.t < rst.t) {
            Vec poc = tmp.poc;
            if (contains(poc)) {
                rst = tmp;
                rst.hit = this;
                rst.type = type;
            }
        }
    }
    return rst;
}

bool AABBox::contains(const Vec &pt) const {
    bool valid = true;
    for (int d = 0; d < 3; d++) {
        if (pt[d] > high[d] + pos[d] + EPS || pt[d] < low[d] + pos[d] - EPS)
            valid = false;
    }
    return valid;
}

void AABBox::make_faces() {
    for (int i = 0; i < 6; i++) {
        Vec normal = Vec(0, 0, 0);
        normal[i / 2] = 1;
        if (i & 1)
            faces[i].set(low[i / 2], normal);
        else
            faces[i].set(high[i / 2], normal);
    }
}

void AABBox::transform(const Vec &pt, double scale) {
    for (int i = 0; i < 3; i++) {
        low[i] = low[i] * scale;
        high[i] = high[i] * scale;
    }
    for (int i = 0; i < 6; i++) {
        Vec normal = Vec(0, 0, 0);
        normal[i / 2] = 1;
        if (i & 1)
            faces[i].set(low[i / 2] + pos[i / 2], normal);
        else
            faces[i].set(high[i / 2] + pos[i / 2], normal);
    }
}