//
// Created by Hong Man Hou on 25/11/2018.
//

#ifndef SCHNERT_OBJECT_H
#define SCHNERT_OBJECT_H
#include "utils.h"

namespace stage {
    enum HitType {
        MISS, INTO, OUTO
    };

    class Object;

    class Ray {
    public:
        Vec src, dir;

        Ray() {}
        Ray(Vec s, Vec d) : src(s), dir(d.unit()) {};

        friend std::ostream &operator <<(std::ostream &out, const Ray &ray) {
            out << "Source is " <<ray.src <<std::endl;
            out << "Dir is " <<ray.dir <<std::endl;
            return out;
        }
    };

    class Intersection {
    public:
        HitType type;
        double t;
        Vec poc;
        Vec normal;
        const Object* hit;
        Intersection(): type(MISS), t(INF_D) {}
    };

    class Object {
    public:
        enum Type {
            SPHERE, PLANE, BEZIER, BOX
        };

        double n;

        Vec pos, emit, color, absorb;
        double diff, refr, spec;

        Object(): diff(0), refr(0), spec(0) {}

        virtual ~Object() = default;

        virtual Type get_type() const = 0;

        virtual Intersection intersect(const Ray &ray) const = 0;

        friend std::ostream &operator <<(std::ostream &fout, const Object &o);
    };

    class Sphere : public Object {
    private:
        double rad;
    public:
        Sphere() {}

        ~Sphere() override = default;

        Type get_type() const override { return Object::SPHERE; }

        Intersection intersect(const Ray &ray) const override;

        Vec normal(const Vec &pt) const;

        friend std::istream &operator >>(std::istream &fin, Sphere &s);

        friend std::ostream &operator <<(std::ostream &fout, const Sphere &s);
    };

    class Plane : public Object {
        // used pos to store vector to the plane from (0,0,0)
    private:
        // normalized normal
        Vec normalized;
        double dist;
    public:
        Plane() {}

        ~Plane() override = default;

        Type get_type() const override { return Object::PLANE; }

        Intersection intersect(const Ray &ray) const override;

        void set(double dist, Vec normalized) {
            int sign = dist > 0 ? 1 : -1;
            this->dist = dist * sign; this->normalized = normalized * (-sign);
            this->pos = normalized * this -> dist;
        }

        friend std::istream &operator >>(std::istream &fin, Plane &s);

        friend std::ostream &operator <<(std::ostream &fout, const Plane &s);

        friend class AABBox;
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

        Intersection intersect(const Ray &ray) const override;

        bool contains(const Vec &pt) const;

        void make_faces();

        friend std::istream &operator >>(std::istream &fin, AABBox &s);

        friend std::ostream &operator <<(std::ostream &fout, const AABBox &s);

        friend class BezierRotational;
    };
}



#endif //SCHNERT_OBJECT_H
