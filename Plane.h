//
// Created by Hong Man Hou on 19/1/2019.
//

#ifndef SCHNERT_PLANE_H
#define SCHNERT_PLANE_H
#include "Object.h"

namespace stage {
    class Plane : public Object {
        // used pos to store vector to the plane from (0,0,0)
    public:
        // normalized normal
        Vec normalized;
        double dist;
    public:
        Plane() {}

        ~Plane() override = default;

        Type get_type() const override { return Object::PLANE; }

        Intersection intersect(const Ray &ray, bool with_BB) const override;

        void set(double dist, Vec normalized) {
            int sign = dist > 0 ? 1 : -1;
            this->dist = dist * sign; this->normalized = normalized * (-sign);
            this->pos = normalized * dist;
        }

        friend std::istream &operator >>(std::istream &fin, Plane &s);

        friend std::ostream &operator <<(std::ostream &fout, const Plane &s);

        friend class AABBox;

        friend class Rectangle;
    };

    class AABBox : public Object {
    private:
        // six faces (origin is (0,0,0))
        Vec low, high;
        Plane faces[6];
    public:
        AABBox(): low(INF_D, INF_D, INF_D), high(-INF_D, -INF_D, -INF_D) {}

        ~AABBox() override = default;

        Type get_type() const override { return Object::BOX; }

        Intersection intersect(const Ray &ray, bool with_BB) const override;

        bool contains(const Vec &pt) const;

        void make_faces();

        void transform(const Vec &pos, double scale);

        friend std::istream &operator >>(std::istream &fin, AABBox &s);

        friend std::ostream &operator <<(std::ostream &fout, const AABBox &s);

        friend class BezierRotational;
    };
}


#endif //SCHNERT_PLANE_H
