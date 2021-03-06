//
// Created by Hong Man Hou on 13/1/2019.
//

#ifndef SCHNERT_BEZIERCURVE_H
#define SCHNERT_BEZIERCURVE_H
#include "utils.h"
#include "Object.h"
#include "Plane.h"

namespace stage {
    class BezierCurve {
    public:
        std::vector<Vec> c_points;
        BezierCurve() {}
        BezierCurve(std::vector<Vec> c_points) : c_points(c_points) {}

        static Vec eval(const BezierCurve &curve, double t);

        static Vec deri(const BezierCurve &curve, double t);
    };

    class BezierRotational : public Object {
    private:
        int Q;
        int order;
        Vec axis, right, up;
        AABBox b_box;
        BezierCurve curve;
    public:
        BezierRotational() {};
        Intersection intersect(const Ray &, bool with_BB) const override;
        Type get_type() const override { return BEZIER; };
        Vec eval(double u, double v) const;
        Vec du(double u, double v) const;
        Vec dv(Vec pt) const;
        double compute_angle(const Vec&pt) const;
        void compute_b_box();
        friend std::istream &operator>>(std::istream &fin, BezierRotational &b);
        friend std::ostream &operator<<(std::ostream &fout, const BezierRotational &o);
    };
}

#endif //SCHNERT_BEZIERCURVE_H
