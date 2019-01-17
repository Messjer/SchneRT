//
// Created by Hong Man Hou on 13/1/2019.
//

#ifndef SCHNERT_BEZIERCURVE_H
#define SCHNERT_BEZIERCURVE_H
#include "utils.h"
#include "Object.h"

namespace stage {
    class BezierCurve {
    public:
        std::vector<Vec> c_points;
        BezierCurve() {}
        BezierCurve(std::vector<Vec> c_points) : c_points(c_points) {}

        static Vec eval(const BezierCurve &curve, double t);

        static Vec deri(const BezierCurve &curve, double t);
    };

    struct int4
    {
        int a, b, c, d;
        int4() :a(0), b(0), c(0), d(0) {}
        int4(int a_, int b_, int c_, int d_) :a(a_), b(b_), c(c_), d(d_) {}
    };

    class BezierRotational : public Object {
    private:
        int order;
        Vec axis;
        AABBox b_box;
        BezierCurve curve;
    public:
        BezierRotational() {};
        Intersection intersect(const Ray &) const override;
        Type get_type() const override { return BEZIER; };
        Vec eval(double u, double v) const;
        Vec du(double u, double v) const;
        Vec dv(Vec pt) const;
        void genObj(int nu, int nv);
        friend std::istream &operator>>(std::istream &fin, BezierRotational &b);
        friend std::ostream &operator<<(std::ostream &fout, const BezierRotational &o);
    };
}

#endif //SCHNERT_BEZIERCURVE_H
