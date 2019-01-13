//
// Created by Hong Man Hou on 25/11/2018.
//

#ifndef SCHNERT_STAGE_H
#define SCHNERT_STAGE_H
#include "Canvas.h"
#include "Object.h"

namespace stage {
    class Camera : public Ray {
    public:
        int w, h;
        double scale, dist;
        Vec3d right, up;
        Camera(Vec3d s, Vec3d d, Vec3d r, double dist, double scale, int w, int h): Ray(s, d), dist(dist), w(w), h(h), right(
                r.unit()), scale(scale) {
            up = dir.cross(right);
            // std::cout <<"Camera Ready!" <<std::endl <<"Located at " <<src <<std::endl << " Up = " <<up  <<std::endl << " Right = " <<right <<std::endl;
        }
        Ray get_ray(double x, double y);
    };

    class Stage {
    public:
        Stage(std::string description);
        ~Stage();
        void set_cam(Camera cam) { this -> eye = cam; }
        void add_object(const Object &object) { objects.push_back(object.clone()); }

        Canvas *ray_trace(int samp = 200, double resl = 1) { return ray_trace(0, eye.h, 0, eye.w, samp, resl); }

        /* Perform raytracing and output a Canvas
         * path tracing core algorithm
         * @Params: 0 <= w1 <= w2 <= eye.w
         * @Params: 0 <= h1 <= h2 <= eye.h
         * @Return: A *Canvas* of the output range, only specified rectangle areas is rendered */
        Canvas *ray_trace(int h1, int h2, int w1, int w2, int samp, double resl);

    // private:
        Camera eye;
        std::vector<Object*> objects;
        Vec3d radiance(const Ray &ray, int depth, unsigned short *Xi);
        Vec3d random_hemi_ray_cos(const Vec3d &normal, unsigned short *Xi);
        std::pair<int,double> intersect(const Ray &ray);

        friend std::ostream &operator <<(std::ostream &out, const Stage &stage);
    };

    static const Vec3d COLOR_BLACK = {};

}

#endif //SCHNERT_STAGE_H
