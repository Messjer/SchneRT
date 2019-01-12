//
// Created by Hong Man Hou on 25/11/2018.
//

#include "Stage.h"
#include "Object.h"
using namespace stage;
using namespace std;

Stage::Stage(string description): eye(Vec3d(50, 52, 295.6), Vec3d(0, -0.042612, -1), Vec3d(1), 140, .00064, 1024, 768) {
    objects.push_back(new Sphere(1e5, Vec3d(1e5 + 1, 40.8, 81.6), COLOR_BLACK, Vec3d(.75, .25, .25), Object::DIFF));//Left
    objects.push_back(new Sphere(1e5, Vec3d(-1e5 + 99, 40.8, 81.6), COLOR_BLACK, Vec3d(.25, .25, .75), Object::DIFF));//Rght
    objects.push_back(new Sphere(1e5, Vec3d(50, 40.8, 1e5), COLOR_BLACK, Vec3d(.75, .75, .75), Object::DIFF));//Back
    objects.push_back(new Sphere(1e5, Vec3d(50, 40.8, -1e5 + 170), COLOR_BLACK, COLOR_BLACK, Object::DIFF));//Frnt
    objects.push_back(new Sphere(1e5, Vec3d(50, 1e5, 81.6), COLOR_BLACK, Vec3d(.75, .75, .75), Object::DIFF));//Botm
    objects.push_back(new Sphere(1e5, Vec3d(50, -1e5 + 81.6, 81.6), COLOR_BLACK, Vec3d(.75, .75, .75), Object::DIFF));//Top
    objects.push_back(new Sphere(16.5, Vec3d(27, 16.5, 47), COLOR_BLACK, Vec3d(1, 1, 1) * .999, Object::SPEC));//Mirr
    objects.push_back(new Sphere(16.5, Vec3d(73, 16.5, 78), COLOR_BLACK, Vec3d(1, 0.5, 0) * .999, Object::REFR));//Glas
    objects.push_back(new Sphere(599.82, Vec3d(50, 681.6 - .27, 81.6), Vec3d(12, 12, 12), COLOR_BLACK, Object::DIFF));//Lite
    objects.push_back(new Sphere(3, Vec3d(73, 16.5, 78), Vec3d(15,15,15), COLOR_BLACK, Object::DIFF));//Lite
    //objects.push_back(new Sphere(3, Vec3d(27, 45, 47), Vec3d(0,15,10), COLOR_BLACK, Object::DIFF));//Lite2
    objects.push_back(new Sphere(3, Vec3d(50, 50, 47), Vec3d(0,3,15), COLOR_BLACK, Object::DIFF));//LiteUp
    objects.push_back(new Sphere(3.2, Vec3d(50, 50, 47), COLOR_BLACK, Vec3d(1,1,1), Object::REFR));//LiteUp
}

Stage::~Stage() {
    for (auto object : objects) {
        delete object;
    }

}

pair<int, double> Stage::intersect(const Ray &ray) {
    double rst = INF_D; int id = -1;
    for (int i = 0; i < objects.size(); i++) {
        double t = objects[i] -> intersect(ray);
        if (t < rst) {
            id = i;
            rst = t;
        }
    }
    return make_pair(id, rst);
}

Vec3d Stage::radiance(const Ray &ray, int depth, unsigned short *Xi) {
    int id; double t;
    std::tie(id, t) = intersect(ray);

    if (id == -1)
        return COLOR_BLACK;

    // cout <<"hit " <<id <<endl;
    const Object * hit = objects[id];

    if (++depth > 5) return hit -> emit;

    // point of contact
    Vec3d poc = ray.src + ray.dir * t;
    assert(hit -> touched(poc));
    Vec3d normal, normal_orig = hit -> normal(Ray(poc, ray.dir));
    bool into = normal_orig.dot(ray.dir) < 0;
    if (!into)
        normal = normal_orig * -1;
    else
        normal = normal_orig;

    /* compute the shadow rays */

    // specular reflection
    switch (hit -> refl) {
        // Specular Reflection
        case Object::REFR: {
            Ray reflect = Ray(poc, (ray.dir - normal * 2 * normal.dot(ray.dir)).unit());
            bool into = normal_orig.dot(ray.dir) < EPS;
            double nc=1, nt=1.5, nnt=into?nc/nt:nt/nc, ddn=ray.dir.dot(normal), cos2t;
            if ((cos2t=1-nnt*nnt*(1-ddn*ddn))<0)    // Total internal reflection
                return hit -> emit + hit -> color * (radiance(reflect,depth,Xi));
            Vec3d tdir = (ray.dir*nnt - normal*(ddn*nnt+sqrt(cos2t))).unit();
            double a=nt-nc, b=nt+nc, R0=a*a/(b*b), c = 1-(into?-ddn:tdir.dot(normal_orig));
            double Re=R0+(1-R0)*c*c*c*c*c,Tr=1-Re,P=.25+.5*Re,RP=Re/P,TP=Tr/(1-P);
            return hit -> emit + hit -> color *(depth>2 ? (erand48(Xi)<P ?   // Russian roulette
                                             radiance(reflect,depth,Xi)*RP:radiance(Ray(poc,tdir),depth,Xi)*TP) :
                                  radiance(reflect,depth,Xi)*Re+radiance(Ray(poc,tdir),depth,Xi)*Tr);
        }
        case Object::SPEC: {
            Ray reflect = Ray(poc, (ray.dir - normal * 2 * normal.dot(ray.dir)).unit());
            return hit->emit + hit->color * radiance(reflect, depth, Xi);
        }
        case Object::DIFF: {
            Ray shadow = Ray(poc, random_hemi_ray_cos(normal, Xi));
            return hit -> emit + hit -> color * radiance(shadow, depth, Xi);
        }
    }
}

Vec3d Stage::random_hemi_ray_cos(const Vec3d &normal, unsigned short *Xi) {
    /* generate random vector obeying cosine distribution */

    Vec3d xx;
    if (fabs(normal.x) > stage::EPS)
        xx = Vec3d(normal.y, -normal.x, 0).unit();
    else
        xx = Vec3d(0, -normal.z, normal.y).unit();
    Vec3d yy = normal.cross(xx);
    // a formula for random direction
    // https://cg.informatik.uni-freiburg.de/course_notes/graphics2_08_renderingEquation.pdf
    double theta = 2 * PI * erand48(Xi);
    double r = erand48(Xi);
    double sr = sqrt(r);
    return (xx * sin(theta) * sr + yy * cos(theta) * sr + normal * sqrt(1 -r)).unit();
}

// path tracing core algorithm
Canvas* Stage::ray_trace(int h1, int h2, int w1, int w2, int samp, double resl) {
    assert(w2 > w1 && w1 >= 0 && w2 <= eye.w && h2 > h1 && h1 >=0 && h2 <= eye.h);
    int newY = (h2 - h1)/resl, newX = (w2 - w1)/resl;
    auto *rst = new Canvas((h2 - h1)/resl, (w2 - w1)/resl);

    // loop over every pixel
    Vec3d light;
    bool done = false;
#pragma omp parallel for schedule(dynamic, 1) private(light) shared(done)
    for (int yi = 0; yi < newY; yi++) {  // rows
        // random state Xi
        unsigned short Xi[3]={0,0,0};
        int y = h1 + yi * resl;
        Xi[2] = y * y * y;
        double pctg = 100.*(y - h1)/((h2 - h1)-1);
        if (pctg > 50 && !done) {
            rst->draw_to_file("temp.ppm");
            done = true;
        }
        fprintf(stderr,"\rRendering (%d spp) %5.2f%%",samp,pctg);
        for (int xi = 0; xi < newX; xi++) {  // cols
            int x = w1 + xi * resl;
            light = COLOR_BLACK;
            for (int s  = 0; s < samp; s++) {
                // cast a ray
                auto ray = eye.get_ray(x, y);
                light = light + radiance(ray, 0, Xi) * (1. / samp);
            }
            rst->add_paint(yi, xi, light);
        }
    }

    return rst;
}

Ray Camera::get_ray(int x, int y) {
    double dx = x - w * .5;
    double dy = y - h * .5;
    Vec3d d = dir + right * (dx * scale) + up * (dy * scale);
    return {src + d * dist, d.unit()};
}