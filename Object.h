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

        double n, refr;

        Vec3d pos, emit, color;
        Refl_Type refl;

        Object(Vec3d p, Vec3d e, Vec3d c, Refl_Type refl, double n) : pos(p), emit(e), color(c), refl(refl), n(n), refr(.7) {}

        virtual ~Object() = default;

        virtual Type get_type() = 0;

        virtual double intersect(const Ray &ray) const = 0;

        virtual Vec3d normal(const Ray &ray) const = 0;

        virtual bool touched(const Vec3d &poc) const = 0;

        virtual Object *clone() const = 0;

        virtual std::string to_string() const = 0;
    };

    class Sphere : public Object {
    private:
        double rad;
    public:
        Sphere(double rad, Vec3d p, Vec3d e, Vec3d c, Refl_Type refl, double n = 1.5) : Object(p, e, c, refl, n), rad(rad) {}

        ~Sphere() override = default;

        Type get_type() override { return Object::SPHERE; }

        double intersect(const Ray &ray) const override;

        Vec3d normal(const Ray &ray) const override;

        bool touched(const Vec3d &poc) const override { return fabs((pos - poc).dot(pos - poc) - rad * rad) < EPS; }

        Sphere *clone() const override { return new Sphere(rad, pos, emit, color, refl); };

        std::string to_string() const override {
            std::stringstream ss;
            ss <<"pos: " <<pos <<std::endl <<rad;
            return ss.str();
        };
    };
}



#endif //SCHNERT_OBJECT_H
