//
// Created by Hong Man Hou on 25/11/2018.
//

#include "Stage.h"
#include "Object.h"
#include "Bezier.h"
#define REFLECT_CNT 5
using namespace stage;
using namespace std;

Stage::Stage(string fname) {
    ifstream fin(fname.c_str());
    string str;
    while (fin >> str) {
        //ignore empty lines and comments
        if (str.length() <= 0 || str[0] == '#') {
            fin.ignore(256, '\n');
            continue;
        } else if (str == "samp") {
            fin >> samp;
        } else if (str == "resl") {
            fin >> resl;
        } else if (str == "eye") {
            fin >> eye;
        } else if (str == "Sphere") {
            auto *s = new Sphere();
            fin >> (*s);
            objects.push_back(s);
        } else if (str == "Plane") {
            auto *obj = new Plane();
            fin >> (*obj);
            objects.push_back(obj);
        } else if (str == "Bezier") {
            auto *obj = new BezierRotational();
            fin >> (*obj);
            objects.push_back(obj);
        } else if (str == "AABB") {
            auto *obj = new AABBox();
            fin >> (*obj);
            objects.push_back(obj);
        } else if (str == "LimitedPlane") {
            auto *obj = new LimitedPlane();
            fin >> (*obj);
            objects.push_back(obj);
        } else {
            cerr << std::string("Unrecognized object : ") + str << endl;
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
    // if (depth == 0 && ray.src.y <35 && ray.src.y > 30 && ray.src.x > 30 && ray.src.x < 35)
    //    return Vec(0,0,1);
    // return hit -> color;
    // return hit -> color;

    if (++depth > REFLECT_CNT) return hit->emit;

    // point of contact
    Vec poc = intersection.poc;

    Vec normal_orig, normal = intersection.normal;
    bool into = intersection.type == INTO;
    if (!into)
        normal_orig = normal * -1;
    else
        normal_orig = normal;

    /* compute the shadow rays */

    Vec color, hit_color = hit -> get_color(poc);
    //Vec hit_color = hit -> color;
    color = hit -> emit;
    //if (depth == 1 && hit -> emit [0] > EPS)
    //    color = hit -> color * .25;


    // refraction
    if (hit->refr > EPS && hit -> n > 0) {
        Ray reflect = Ray(poc, (ray.dir - normal * 2 * normal.dot(ray.dir)).unit());
        double nc = 1, nt = hit -> n, nnt = into ? nc / nt : nt / nc, ddn = ray.dir.dot(normal), cos2t;
        if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0)    // Total internal reflection
            return hit->emit + hit->color * (radiance(reflect, depth, Xi));
        Vec tdir = (ray.dir * nnt - normal * (ddn * nnt + sqrt(cos2t))).unit();
        Ray trans = Ray(poc, tdir);
        double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : tdir.dot(normal_orig));
        double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P, TP = Tr / (1 - P);
        Vec to_add = hit_color;
        Vec alpha = {1,1,1};
        if (!into && hit -> get_type() != Object::BEZIER)
            alpha = hit->absorb.exp(intersection.t);
        if (depth < 2)
            to_add = to_add * radiance(reflect, depth, Xi) * Re + radiance(trans, depth, Xi) * Tr * alpha;
        else if (erand48(Xi) < P)
            to_add = to_add * radiance(reflect, depth, Xi) * RP;
        else
            to_add = to_add * radiance(trans, depth, Xi) * TP * alpha;
        color = color + to_add;
    }

    // specular
    if (hit->spec > EPS) {
        Ray reflect = Ray(poc, (ray.dir - normal * 2 * normal.dot(ray.dir)).unit());
        color = color + hit_color * radiance(reflect, depth, Xi) * hit -> spec;
    }

    // diffuse
    if (hit->diff > EPS) {
        Ray shadow = Ray(poc, random_hemi_ray_cos(normal, Xi));
        color = color + hit_color * radiance(shadow, depth, Xi) * hit -> diff;
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
Canvas * Stage::ray_trace(int h1, int h2, int w1, int w2) {
    assert(w2 > w1 && w1 >= 0 && w2 <= eye.w && h2 > h1 && h1 >=0 && h2 <= eye.h);
    int new_y = (h2 - h1)/resl, new_x = (w2 - w1)/resl;
    auto *rst = new Canvas((h2 - h1)/resl, (w2 - w1)/resl);

    // loop over every pixel
    Vec light;
    bool done = false;
#pragma omp parallel for schedule(dynamic, 1) private(light) shared(done)
    for (int yi = 0; yi < new_y; yi++) {  // rows
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
        for (int xi = 0; xi < new_x; xi++) {  // cols
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
    std::istream &operator >>(std::istream &fin, Camera &eye) {
        string str;
        while(fin >> str) {
            if (str == "end") break;
            if (str == "pos") fin >> eye.src;
            else if (str == "dir") fin >> eye.dir;
            else if (str == "dist") fin >> eye.dist;
            else if (str == "w") fin >> eye.w;
            else if (str == "h") fin >> eye.h;
            else if (str == "scale") fin >>eye.scale;
            else if (str == "r") {
                fin >>eye.right;
                eye.right.unit();
            }
            else {
                cerr <<string("Unrecognized field: ") + str + " for camera!" <<endl;
                exit(-1);
            }
        }
        eye.dir.unit();
        eye.up = eye.dir.cross(eye.right);
        return fin;
    }
}
