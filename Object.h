//
// Created by Hong Man Hou on 25/11/2018.
//

#ifndef SCHNERT_OBJECT_H
#define SCHNERT_OBJECT_H
#include "utils.h"

namespace stage {
    //using std::operator<<;
    //using std::operator>>;

    class Ray {
    public:
        Vec3d src, dir;

        Ray(Vec3d s, Vec3d d) : src(s), dir(d.unit()) {};

        friend std::ostream &operator <<(std::ostream &out, const Ray &ray) {
            out << "Source is " <<ray.src <<std::endl;
            out << "Dir is " <<ray.dir <<std::endl;
            return out;
        }
    };

    class Object {
    public:
        enum Type {
            SPHERE
        };

        double n;

        Vec3d pos, emit, color;
        double diff, refr, spec;

        Object() {}

        virtual ~Object() = default;

        virtual Type get_type() const = 0;

        virtual double intersect(const Ray &ray) const = 0;

        virtual Vec3d normal(const Ray &ray) const = 0;

        virtual bool touched(const Vec3d &poc) const = 0;

        virtual Object *clone() const = 0;

        friend std::ostream &operator <<(std::ostream &fout, const Object &o);
    };

    class Sphere : public Object {
    private:
        double rad;
    public:
        Sphere() {}

        ~Sphere() override = default;

        Type get_type() const override { return Object::SPHERE; }

        double intersect(const Ray &ray) const override;

        Vec3d normal(const Ray &ray) const override;

        bool touched(const Vec3d &poc) const override { return fabs((pos - poc).dot(pos - poc) - rad * rad) / rad < EPS; }

        Sphere *clone() const override {
            Sphere *rst = new Sphere();
            memcpy(rst, this, sizeof(Sphere));
            return rst;
        };

        friend std::istream &operator >>(std::istream &fin, Sphere &s);

        friend std::ostream &operator <<(std::ostream &fout, const Sphere &s);
    };
}



#endif //SCHNERT_OBJECT_H
