//
// Created by Hong Man Hou on 25/11/2018.
//

#ifndef SCHNERT_STAGE_H
#define SCHNERT_STAGE_H
#include "Canvas.h"
#include "Object.h"

namespace stage {
    class Ray {
    public:
        Vec3d src, dir;

        Ray(Vec3d s, Vec3d d) : src(s), dir(d.norm()) {};

        friend std::ostream &operator <<(std::ostream &out, const Ray &ray) {
            out << "Source is " <<ray.src <<std::endl;
            out << "Dir is " <<ray.dir <<std::endl;
            return out;
        }
    };

    class Camera : public Ray {
    public:
        int w, h;
        double scale, dist;
        Vec3d right, up;
        Camera(Vec3d s, Vec3d d, Vec3d r, double dist, double scale, int w, int h): Ray(s, d), dist(dist), w(w), h(h), right(r.norm() * scale), scale(scale) {
            up = dir.cross(right) * scale;
            std::cout <<"Camera Ready!" <<std::endl <<"Located at " <<src << " Up = " <<up << " Right = " <<right <<std::endl;
        }
    };

    class Stage {
    public:
        Stage();
        ~Stage();
        void setCamera(Camera cam) { this -> cam = cam; }
        void addObject(const Object &object) { objects.push_back(object.clone()); }

        Canvas *RayTrace() { return RayTrace(0, cam.h, 0, cam.w); }

        /* Perform raytracing and output a Canvas
         * path tracing core algorithm
         * @Params: 0 <=w1 <= w2 <= cam.w
         * @Params: 0 <=h1 <= h2 <= cam.h
         * @Return: A *Canvas* of the output range, only specified rectangle areas is rendered */
        Canvas *RayTrace(int w1, int w2, int h1, int h2);

    private:
        Camera cam;
        std::vector<Object*> objects;
        Vec3d radiance(const Ray &ray, int depth);
        std::pair<int,double> intersect(const Ray &ray);
    };

    static const Vec3d COLOR_BLACK = {};

}

#endif //SCHNERT_STAGE_H
