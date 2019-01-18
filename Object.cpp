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
            case Object::BOX:
                fout << (*((AABBox *) &o)) <<endl;
                break;
            case Object::LIMITED_PLANE:
                fout << (*((LimitedPlane *) &o)) <<endl;
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

    std::istream &operator >>(std::istream &fin, LimitedPlane &s) {
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

    std::ostream &operator <<(std::ostream &fout, const LimitedPlane &s) {
        fout <<"Limited Plane:" <<endl;
        fout <<"Underlying plane is "<<s.plane <<endl;
        fout <<"low is " <<s.low <<endl;
        fout <<"high is " <<s.high <<endl;
        fout <<"width is " <<s.width <<endl;
        return fout;
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
    if (t > 2 * EPS)
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
    HitType type =  contains(ray.src) ? OUTO : INTO;
    for (int i = 0; i < 6; i++) {
        Intersection tmp = faces[i].intersect(ray);
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

Intersection LimitedPlane::intersect(const Ray &ray) const {
    Intersection tmp = plane.intersect(ray);
    if (tmp.type == MISS || !contains(tmp.poc)) {
        tmp.type = MISS;
        return tmp;
    }
    tmp.hit = this;
    return tmp;
}

bool LimitedPlane::contains(const Vec &pt) const {
    bool valid = true;
    for (int d = 0; d < 3; d++) {
        if (pt[d] > high[d] + pos[d] + EPS || pt[d] < low[d] + pos[d] - EPS)
            valid = false;
    }
    return valid;
}

Vec LimitedPlane::get_color(const Vec &pt) const {
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
            }
    } else
        return color;
}

double LimitedPlane::get_offset(const Vec &pt, int d) const {
    return (pt[d] - low[d]) / width[d];
}

double LimitedPlane::get_offset_unscaled(const Vec &pt, int d) const {
    return (pt[d] - low[d]);
}