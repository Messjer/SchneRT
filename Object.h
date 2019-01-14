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
            SPHERE, PLANE, BEZIER
        };

        double n;

        Vec pos, emit, color;
        double diff, refr, spec;

        Object() {}

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
        Vec N, normalized;
        double dist;
    public:
        Plane() {}

        ~Plane() override = default;

        Type get_type() const override { return Object::PLANE; }

        Intersection intersect(const Ray &ray) const override;

        friend std::istream &operator >>(std::istream &fin, Plane &s);

        friend std::ostream &operator <<(std::ostream &fout, const Plane &s);
    };
}



#endif //SCHNERT_OBJECT_H
