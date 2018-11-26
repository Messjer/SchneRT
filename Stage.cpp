//
// Created by Hong Man Hou on 25/11/2018.
//

#include "Stage.h"
#include "Object.h"
using namespace stage;
using namespace std;

Stage::Stage(): cam(Vec3d(50, 52, 295.6), Vec3d(0, -0.042612, -1), Vec3d(1), 140, .5135, 1024, 768) {
    objects.push_back(new Sphere(1e5, Vec3d(1e5 + 1, 40.8, 81.6), COLOR_BLACK, Vec3d(.75, .25, .25), Object::DIFF));//Left
    objects.push_back(new Sphere(1e5, Vec3d(-1e5 + 99, 40.8, 81.6), COLOR_BLACK, Vec3d(.25, .25, .75), Object::DIFF));//Rght
    objects.push_back(new Sphere(1e5, Vec3d(50, 40.8, 1e5), COLOR_BLACK, Vec3d(.75, .75, .75), Object::DIFF));//Back
    objects.push_back(new Sphere(1e5, Vec3d(50, 40.8, -1e5 + 170), COLOR_BLACK, COLOR_BLACK, Object::DIFF));//Frnt
    objects.push_back(new Sphere(1e5, Vec3d(50, 1e5, 81.6), COLOR_BLACK, Vec3d(.75, .75, .75), Object::DIFF));//Botm
    objects.push_back(new Sphere(1e5, Vec3d(50, -1e5 + 81.6, 81.6), COLOR_BLACK, Vec3d(.75, .75, .75), Object::DIFF));//Top
    objects.push_back(new Sphere(16.5, Vec3d(27, 16.5, 47), COLOR_BLACK, Vec3d(1, 1, 1) * .999, Object::SPEC));//Mirr
    objects.push_back(new Sphere(16.5, Vec3d(73, 16.5, 78), COLOR_BLACK, Vec3d(1, 1, 1) * .999, Object::REFR));//Glas
    objects.push_back(new Sphere(600, Vec3d(50, 681.6 - .27, 81.6), Vec3d(12, 12, 12), COLOR_BLACK, Object::DIFF));//Lite

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

Vec3d Stage::radiance(const Ray &ray, int depth) {
    int id; double t;
    std::tie(id, t) = intersect(ray);

    if (id == -1)
        return COLOR_BLACK;

    // cout <<"hit " <<id <<endl;
    const Object * hit = objects[id];

    if (++depth > 5) return hit -> emit;

    // compute the shadow rays

    // specular reflection

    // point of contact
    Vec3d poc = ray.src + ray.dir * t;
    assert(hit -> touched(poc));

    Vec3d normal = hit -> normal(poc);
    Ray reflect = Ray(poc, (ray.dir - normal * 2 * normal.dot(ray.dir)).norm());
    return hit -> emit + hit -> color * radiance(reflect, depth);
}

// path tracing core algorithm
Canvas* Stage::RayTrace(int h1, int h2, int w1, int w2) {
    assert(w2 > w1 && w1 >= 0 && w2 <= cam.w && h2 > h1 && h1 >=0 && h2 <= cam.h);
    auto *rst = new Canvas(h2 - h1, w2 - w1);

    // loop over every pixel
    for (int y = h1; y < h2; y++) {  // rows
        for (int x = w1; x < w2; x++) {  // cols
            // cast a ray
            double dx = x - cam.w * .5;
            double dy = y - cam.h * .5;
            Vec3d d = cam.dir * cam.dist + cam.right * dx + cam.up * dy;

            auto ray = Ray(cam.src + d * cam.scale, d.norm());
            auto light = radiance(ray, 0);
            // printf("Computed %ld %ld %ld\n", lround(light.x), lround(light.y), lround(light.z));
            rst -> draw(y, x, light);
            if (y == h1 && (x == w1 || x == w2 - 1))
                cout <<ray <<endl;
        }
    }

    return rst;
}