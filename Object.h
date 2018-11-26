//
// Created by Hong Man Hou on 25/11/2018.
//

#ifndef SCHNERT_OBJECT_H
#define SCHNERT_OBJECT_H
#include "utils.h"

namespace stage {
    class Ray;

    class Object {
    public:
        enum Type {
            SPHERE
        };
        enum Refl_Type {
            DIFF, SPEC, REFR
        };


        Vec3d pos, emit, color;
        Refl_Type refl;

        Object(Vec3d p, Vec3d e, Vec3d c, Refl_Type refl) : pos(p), emit(e), color(c), refl(refl) {}

        virtual ~Object() = default;

        virtual Type getType() = 0;

        virtual double intersect(const Ray &ray) const = 0;

        virtual Vec3d normal(const Vec3d &pos) const = 0;

        virtual bool touched(const Vec3d &poc) const = 0;

        virtual Object *clone() const = 0;
    };

    class Sphere : public Object {
    private:
        double rad;
    public:
        Sphere(double rad, Vec3d p, Vec3d e, Vec3d c, Refl_Type refl) : Object(p, e, c, refl), rad(rad) {}

        ~Sphere() override = default;

        Type getType() override { return Object::SPHERE; }

        double intersect(const Ray &ray) const override;

        Vec3d normal(const Vec3d &poc) const override { return (poc - pos).norm(); }

        bool touched(const Vec3d &poc) const override { return fabs((pos - poc).dot(pos - poc) - rad * rad) < EPS; }

        Sphere *clone() const override { return new Sphere(rad, pos, emit, color, refl); };
    };
}



#endif //SCHNERT_OBJECT_H
