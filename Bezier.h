//
// Created by Hong Man Hou on 13/1/2019.
//

#ifndef SCHNERT_BEZIERCURVE_H
#define SCHNERT_BEZIERCURVE_H
#include "utils.h"
#include "Object.h"

namespace stage {
    class BezierCurve {
    private:
        std::vector<Vec> c_points;
    public:
        BezierCurve(std::vector<Vec> c_points) : c_points(c_points) {}

        static Vec eval(BezierCurve curve, double t);

        static Vec deri(BezierCurve curve, double t);
    };

    struct int4
    {
        int a, b, c, d;
        int4() :a(0), b(0), c(0), d(0) {}
        int4(int a_, int b_, int c_, int d_) :a(a_), b(b_), c(c_), d(d_) {}
    };

    class BezierRotational : public Object {
    private:
        int nu, nv;
        Vec axis;
        //Box b_box;
        std::vector<Vec> points;
    public:
        BezierRotational() {};
        Intersection intersect(const Ray &) const override;
        Type get_type() const override { return BEZIER; };
        Vec eval(double nu, double nv);
        void genObj(int nu, int nv);
        friend std::istream &operator>>(std::istream &fin, BezierRotational &b);
        friend std::ostream &operator<<(std::ostream &fout, const BezierRotational &o);
    };
}

#endif //SCHNERT_BEZIERCURVE_H
