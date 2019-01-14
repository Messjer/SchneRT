//
// Created by Hong Man Hou on 25/11/2018.
//

#include "Stage.h"
#include "Object.h"
#include "Bezier.h"
using namespace stage;
using namespace std;

Stage::Stage(string fname): eye(Vec(50, 52, 295.6), Vec(0, -0.042612, -1), Vec(1), 140, .00064, 1024, 768) {
    ifstream fin(fname.c_str());
    string str;
    while (fin >> str) {
        //ignore empty lines and comments
        if (str.length() <= 0 || str[0] == '#') {
            fin.ignore(256, '\n');
            continue;
        }
        if (str == "Sphere") {
            auto *s = new Sphere();
            fin >>(*s);
            objects.push_back(s);
        } else if (str == "Plane") {
            auto *obj = new Plane();
            fin >>(*obj);
            objects.push_back(obj);
        } else if (str == "Bezier") {
            auto *obj = new BezierRotational();
            fin >>(*obj);
            obj -> genObj(100, 100);
            objects.push_back(obj);
        } else{
            cerr <<std::string("Unrecognized object : ") + str <<endl;
            exit(-1);
        }

    }
}

Stage::~Stage() {
    for (auto object : objects) {
        delete object;
    }

}

Intersection Stage::intersect(const Ray &ray) {
    Intersection rst;
    for (int i = 0; i < objects.size(); i++) {
        Intersection tmp = objects[i] -> intersect(ray);
        if (tmp.type != MISS && tmp.t < rst.t) {
            rst = tmp;
        }
    }
    return rst;
}

Vec Stage::radiance(const Ray &ray, int depth, unsigned short *Xi) {
    Intersection intersection = intersect(ray);

    if (intersection.type == MISS)
        return COLOR_BLACK;

    const Object *hit = intersection.hit;

    if (++depth > 5) return hit->emit;

    // point of contact
    Vec poc = intersection.poc;

    /* if (!(hit -> touched(poc))) {
        cerr<< poc << " does not touch " <<(*hit) <<endl;
        exit(-1);
    } */

    Vec normal, normal_orig = intersection.normal;
    bool into = normal_orig.dot(ray.dir) < 0;
    if (!into)
        normal = normal_orig * -1;
    else
        normal = normal_orig;

    /* compute the shadow rays */

    Vec color = hit -> emit;

    // refraction
    if (hit->refr > EPS) {
        Ray reflect = Ray(poc, (ray.dir - normal * 2 * normal.dot(ray.dir)).unit());
        bool into = normal_orig.dot(ray.dir) < EPS;
        double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc, ddn = ray.dir.dot(normal), cos2t;
        if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0)    // Total internal reflection
            return hit->emit + hit->color * (radiance(reflect, depth, Xi));
        Vec tdir = (ray.dir * nnt - normal * (ddn * nnt + sqrt(cos2t))).unit();
        double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : tdir.dot(normal_orig));
        double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P, TP = Tr / (1 - P);
        color = color +
                hit->color * (depth > 2 ? (erand48(Xi) < P ?   // Russian roulette
                                                      radiance(reflect, depth, Xi) * RP :
                                                      radiance(Ray(poc, tdir), depth, Xi) * TP) :
                                         radiance(reflect, depth, Xi) * Re + radiance(Ray(poc, tdir), depth, Xi) * Tr)
                         ;
    }

    // specular
    if (hit->spec > EPS) {
        Ray reflect = Ray(poc, (ray.dir - normal * 2 * normal.dot(ray.dir)).unit());
        color = color + hit->color * radiance(reflect, depth, Xi) ;
    }

    // diffuse
    if (hit->diff > EPS) {
        Ray shadow = Ray(poc, random_hemi_ray_cos(normal, Xi));
        color = color + hit->color * radiance(shadow, depth, Xi) ;
    }

    return color;

}

Vec Stage::random_hemi_ray_cos(const Vec &normal, unsigned short *Xi) {
    /* generate random vector obeying cosine distribution */

    Vec xx;
    if (fabs(normal.x) > stage::EPS)
        xx = Vec(normal.y, -normal.x, 0).unit();
    else
        xx = Vec(0, -normal.z, normal.y).unit();
    Vec yy = normal.cross(xx);
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
    Vec light;
    bool done = false;
#pragma omp parallel for schedule(dynamic, 1) private(light) shared(done)
    for (int yi = 0; yi < newY; yi++) {  // rows
        // random state Xi
        unsigned short Xi[3]={0,0,0};
        int y = h1 + yi * resl;
        Xi[2] = y * y * y;
        double pctg = 100.*(y - h1)/((h2 - h1)-1);
        if (pctg > 50 && !done) {
            rst->draw_to_file("temp.png");
            done = true;
        }
        fprintf(stderr,"\rRendering (%d spp) %5.2f%%",samp,pctg);
        for (int xi = 0; xi < newX; xi++) {  // cols
            int x = w1 + xi * resl;
            light = COLOR_BLACK;
            for (int s  = 0; s < samp; s++) {
                // super sampling 2x2
                for (int sy=0; sy<2; sy++)     // 2x2 subpixel rows
                    for (int sx=0; sx<2; sx++) {
                        // cast a ray
                        double r1=2*erand48(Xi), dx=r1<1 ? sqrt(r1)-1: 1-sqrt(2-r1);
                        double r2=2*erand48(Xi), dy=r2<1 ? sqrt(r2)-1: 1-sqrt(2-r2);
                        auto ray = eye.get_ray((sx+.5 + dx)/2 + x, (sy+.5 + dy)/2 + y);
                        light = light + radiance(ray, 0, Xi) * (1. / samp) * .25;
                    }
            }
            rst->add_paint(yi, xi, light);
        }
    }

    return rst;
}

Ray Camera::get_ray(double x, double y) {
    double dx = x - w * .5;
    double dy = y - h * .5;
    Vec d = dir + right * (dx * scale) + up * (dy * scale);
    return {src + d * dist, d.unit()};
}

namespace stage {
    std::ostream &operator<<(std::ostream &out, const Stage &stage) {
        //cout <<*stage.objects[0] <<endl;
        for (Object *obj: stage.objects) {
            out << (*obj) << endl;
        }
        return out;
    }
}