//
// Created by Hong Man Hou on 25/11/2018.
//

#include "Stage.h"

using namespace stage;
using namespace std;

double Sphere::intersect(const stage::Ray &ray) const {
    // Solve (td + src - pos)^2 = R^2
    // i.e. t^2 d.d + 2t(src - pos).d + (src - pos).(src - pos) = R^2
    // By . we mean dot product
    Vec3d sp = pos - ray.src;
    double b = sp.dot(ray.dir);
    // cout <<ray.dir <<endl;
    double t = 0, delta = b * b - sp.dot(sp) + rad * rad;
    //cout <<delta <<endl;

    // no solution
    if (delta < 0) return INF_D;
    else delta = sqrt(delta);


    if ((t = b - delta) > EPS)
        return t;
    else if ((t = b + delta) > EPS)
        return t;
    else
        return INF_D;
}
