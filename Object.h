//
// Created by Hong Man Hou on 25/11/2018.
//

#ifndef SCHNERT_OBJECT_H
#define SCHNERT_OBJECT_H
#include "Canvas.h"

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
            SPHERE, PLANE, BEZIER, BOX, LIMITED_PLANE
        };

        double n;

        Vec pos, emit, color, absorb;
        double diff, refr, spec;
        Canvas *texture;

        Object(): diff(0), refr(0), spec(0), texture(NULL) {}

        virtual ~Object() {
            delete texture;
        }

        virtual Type get_type() const = 0;

        virtual Vec get_color(const Vec &pt) const { return color; }

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
    public:
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
            this->pos = normalized * dist;
        }

        friend std::istream &operator >>(std::istream &fin, Plane &s);

        friend std::ostream &operator <<(std::ostream &fout, const Plane &s);

        friend class AABBox;

        friend class LimitedPlane;
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

        void transform(const Vec &pos, double scale);

        friend std::istream &operator >>(std::istream &fin, AABBox &s);

        friend std::ostream &operator <<(std::ostream &fout, const AABBox &s);

        friend class BezierRotational;
    };

    // Planes are recommended to be axis-aligned for displaying textures
    class LimitedPlane: public Object{
        Vec low, high, width;
        Plane plane;
        double scaled = 0;
    public:
        Type get_type() const override { return Object::LIMITED_PLANE; }
        Intersection intersect(const Ray &ray) const override;

        friend std::istream &operator >>(std::istream &fin, LimitedPlane &s);

        friend std::ostream &operator <<(std::ostream &fout, const LimitedPlane &s);

        bool contains(const Vec &pt) const;

        Vec get_color(const Vec &pt) const override;

        // get offset in dimension d
        double get_offset(const Vec &pt, int d) const;

        double get_offset_unscaled(const Vec &pt, int d) const;
    };
}



#endif //SCHNERT_OBJECT_H
