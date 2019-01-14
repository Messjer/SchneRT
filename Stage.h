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
        Vec right, up;
        Camera() {}
        Camera(Vec s, Vec d, Vec r, double dist, double scale, int w, int h): Ray(s, d), dist(dist), w(w), h(h), right(
                r.unit()), scale(scale) {
            up = dir.cross(right);
            // std::cout <<"Camera Ready!" <<std::endl <<"Located at " <<src <<std::endl << " Up = " <<up  <<std::endl << " Right = " <<right <<std::endl;
        }
        Ray get_ray(double x, double y);

        friend std::istream &operator >>(std::istream &out, Camera &eye);
    };

    class Stage {
    public:
        int reflect_cnt = 0;
        Stage(std::string description);
        ~Stage();

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
        Vec radiance(const Ray &ray, int depth, unsigned short *Xi);
        Vec random_hemi_ray_cos(const Vec &normal, unsigned short *Xi);
        Intersection intersect(const Ray &ray);

        friend std::ostream &operator <<(std::ostream &out, const Stage &stage);
    };

    static const Vec COLOR_BLACK = {};

}

#endif //SCHNERT_STAGE_H
