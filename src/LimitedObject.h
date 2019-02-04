//
// Created by Hong Man Hou on 19/1/2019.
//

#ifndef SCHNERT_LIMITEDOBJECT_H
#define SCHNERT_LIMITEDOBJECT_H
#include "Object.h"
#include "Plane.h"

namespace stage {
    // Planes are recommended to be axis-aligned for displaying textures
    class Rectangle: public Object {
        Vec low, high, width;
        Plane plane;
        double scaled = 0;
    public:
        Type get_type() const override { return Object::RECT; }
        Intersection intersect(const Ray &ray, bool with_BB) const override;

        friend std::istream &operator >>(std::istream &fin, Rectangle &s);

        friend std::ostream &operator <<(std::ostream &fout, const Rectangle &s);

        bool contains(const Vec &pt) const;

        Vec get_color(const Vec &pt) const override;

        // get offset in dimension d
        double get_offset(const Vec &pt, int d) const;

        double get_offset_unscaled(const Vec &pt, int d) const;
    };

    class Disk: public Object {
        Plane plane;
        double rad;
    public:
        Type get_type() const override { return Object::DISK; }
        Intersection intersect(const Ray &ray, bool with_BB) const override;

        friend std::istream &operator >>(std::istream &fin, Disk &s);

        friend std::ostream &operator <<(std::ostream &fout, const Disk &s);

        bool contains(const Vec &pt) const;
    };

    class HoledDisk: public Object {
        Plane plane;
        double rad_in, rad_out;
    public:
        Type get_type() const override { return Object::HOLED_DISK; }
        Intersection intersect(const Ray &ray, bool with_BB) const override;

        friend std::istream &operator >>(std::istream &fin, HoledDisk &s);

        friend std::ostream &operator <<(std::ostream &fout, const HoledDisk &s);

        bool contains(const Vec &pt) const;
    };
}


#endif //SCHNERT_LIMITEDOBJECT_H
