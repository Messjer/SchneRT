//
// Created by Hong Man Hou on 13/1/2019.
//

#include "Bezier.h"
#include "Gauss.h"
// ITER = 6 and ATTEMPT = 12 is good
#define NEWTON_ITER 5
#define NEWTON_ATTEMPT 10
#define NEWTON_DELTA 5e-5

using namespace std;
using namespace stage;

Vec BezierCurve::eval(const BezierCurve &curve, double t) {
    // bezier curve evaluation using de Casteljau's algo
    if (curve.c_points.size() == 1)
        return curve.c_points[0];
    int size = curve.c_points.size();
    vector<Vec> new_points(size - 1);
    for (int i = 0; i < size - 1; i++)
        new_points[i] = curve.c_points[i] * (1 - t) + curve.c_points[i + 1] * t;
    BezierCurve reduced = BezierCurve(new_points);
    return eval(reduced, t);
}

Vec BezierCurve::deri(const BezierCurve &curve, double t) {
    int size = curve.c_points.size();
    vector<Vec> new_points(size - 1);
    for (int i = 0; i < size - 1 ; i++)
        new_points[i] = (curve.c_points[i + 1] - curve.c_points[i]) * (size - 1);
    BezierCurve reduced = BezierCurve(new_points);
    return eval(reduced, t);
}

// this function is copied from Rain's PPM, used for debug only
void BezierRotational::genObj(int nu, int nv)
{
    float du = 1.0f / (nu - 1), dv = 1.0f / (nv - 1);
    vector<Vec> points(0);
    vector<int4> meshes(0);
    int pid = 0;
    float u, v;
    int i, j;
    for (u = 0.0f, i = 0; i <= nu; u += du, i++)
    {
        for (v = 0.0f, j = 0; j <= nv; v += dv, j++)
        {
            points.push_back(eval(u, v));
            pid++;
            if (i != 0 && j != 0)
                meshes.push_back(int4(pid - nv - 1, pid - nv, pid, pid - 1));
        }
    }
    FILE *fp = fopen("BezierMesh.obj", "w");
    for (int i = 0; i < int(points.size()); i++)
        fprintf(fp, "v %f %f %f\n", points[i].x, points[i].y, points[i].z);
    for (int i = 0; i < int(meshes.size()); i++)
        fprintf(fp, "f %d %d %d %d\n", meshes[i].a, meshes[i].b, meshes[i].c, meshes[i].d);
    fclose(fp);
}

Vec BezierRotational::eval(double u, double v) const {
    Vec point = BezierCurve::eval(curve, u);
    point = point.rotate(axis, 2 * PI * v) + pos;
    return point;
}

Vec BezierRotational::du(double u, double v) const {
    Vec deriv = BezierCurve::deri(curve, u);
    deriv = deriv.rotate(axis, 2 * PI * v);
    return deriv;
}

Vec BezierRotational::dv(Vec pt) const {
    pt = pt - pos;
    Vec d = axis.cross(pt).unit();
    d = d * sqrt(pt.norm() * pt.norm() - (pt.dot(axis))*pt.dot(axis)) * 2 * PI;
    return d;
}

void BezierRotational::compute_b_box() {
    double max_abs[3] = {};
    for (int i = 0; i < curve.c_points.size(); i++) {
        if (abs(curve.c_points[i][0]) > max_abs[0]) {
            max_abs[0] = abs(curve.c_points[i][0]);
        }
        if (curve.c_points[i][1] < b_box.low[1])
            b_box.low[1] = curve.c_points[i][1];
        if (curve.c_points[i][1] > b_box.high[1])
            b_box.high[1] = curve.c_points[i][1];
    }
    //cout <<max_abs[0];
    b_box.low[0] = -max_abs[0];
    b_box.high[0] = max_abs[0];
    b_box.low[2] = -max_abs[0];
    b_box.high[2] = max_abs[0];
    for (int d = 0; d < 3; d++)
    {
        b_box.low[d] += pos[d];
        b_box.high[d] += pos[d];
    }
    b_box.color = Vec(1,0,0);
    b_box.diff = 1;
    b_box.make_faces();
}

Intersection BezierRotational::intersect(const Ray &ray) const {
    Intersection rst;

    Intersection with_box = b_box.intersect(ray);

    if (with_box.type == MISS) return rst;

    double ans_t = INF_D;
    Vec ans_du, ans_dv;

    // only if with_box is not miss will we use Newton's method
    Vec X;

    // t, u, v respectively
    Vec src = ray.src, dir = ray.dir;
    bool found = false;
    for (int q = 0; q < NEWTON_ATTEMPT; q++) {
        Vec du, dv, f;

        // first guess
        X = Vec(with_box.t, drand48(), drand48());
        int cnt = 0; // number of increasing distance
        double last_dist = INF_D;

        for (int i = 0; i < NEWTON_ITER; i++) {
            // one iteration of newton's method
            Vec p = eval(X[1], X[2]);
            f = src + dir * X[0] - p;
            du = this->du(X[1], X[2]);
            dv = this->dv(p);
            // By -J(X - X0) = f
            Vec d_x = Gauss::solve(dir * (-1) , du, dv, f);
            X = X + d_x;
            double dist = f.inf_norm();
            if (last_dist < dist) {
                cnt++;
                if (cnt >= 2) {
                    //cout <<"killed " << i <<endl;
                    break;
                }
            } else cnt = 0;
            if (X[0] > EPS && X[1] > EPS && X[2] > EPS && X[1] < 1 - EPS && X[2] < 1 - EPS
                && ((last_dist = dist) < NEWTON_DELTA)) {
                found = true;
                break;
            }
        }
        double t = X[0];
        if (found && t < ans_t) {
            //cout <<t <<endl;
            Vec p = eval(X[1], X[2]);
            du = this->du(X[1], X[2]);
            dv = this->dv(p);
            ans_t = t;
            ans_du = du;
            ans_dv = dv;
            break;
        }
    }

    if (ans_t < INF_D ) {
        rst.t = ans_t;
        rst.normal = ans_du.cross(ans_dv).unit();
        rst.type = dir.dot(rst.normal) > EPS ? OUTO : INTO;
        rst.normal = rst.normal * (rst.type == INTO ? 1 : -1);
        rst.poc = src + dir * rst.t;
        //cout <<rst.poc <<endl;
        rst.hit = this;
        return rst;
    }

    return rst;
}

namespace stage {
    std::istream &operator>>(std::istream &fin, BezierRotational &b) {
        string str;
        double scale = 1;
        while (fin >> str) {
            if (str == "end") break;
            if (str == "n") fin >> b.n;
            else if (str == "pos") fin >> b.pos;
            else if (str == "controls") {
                if (b.n == 0) {
                    cerr <<string("Please first specify number of control points!") <<endl;
                    exit(-1);
                }
                vector<Vec> upoints;
                Vec point;
                upoints.clear();
                for (int j = 0; j < b.n; j++) {
                    fin >> point;
                    upoints.push_back(point);
                }
                b.curve = BezierCurve(upoints);
            }
            else if (str == "diff") fin >> b.diff;
            else if (str == "scale") fin >> scale;
            else if (str == "spec") fin >> b.spec;
            else if (str == "refr") fin >> b.refr;
            else if (str == "color") fin >> b.color;
            else if (str == "axis") fin >> b.axis;
            else if (str == "emit") fin >> b.emit;
            else if (str == "AABB") {
                fin >>b.b_box;
            } else {
                cerr <<string("Unrecognized field: ") + str + " for Bezier rotational!" <<endl;
                exit(-1);
            }
        }
        for (int j = 0; j < b.n; j++)
            b.curve.c_points[j] = b.curve.c_points[j] * scale;
        // use diff == 1 to signify enterd AABB
        if (b.b_box.diff < EPS)
            b.compute_b_box();
        else {
            b.b_box.transform(b.pos, scale);
        }
        return fin;
    }

    std::ostream &operator<<(std::ostream &fout, const BezierRotational &o) {
        fout << "Beizer rotational at: " <<o.pos << endl;
        fout << "Control Points: " <<endl;
        for (int i = 0; i < o.curve.c_points.size(); i++) {
            fout <<o.curve.c_points[i] <<endl;
        }
        fout << "Axis : " <<o.axis;
        fout <<endl;
        fout << "Bounding box is " <<o.b_box <<endl;
        fout << "     --------    " <<endl;
        fout << "Color : " << o.color << " , ";
        fout << "Emit : " << o.emit << " , ";
        fout << "Refr : " << o.refr << " , ";
        fout << "Spec : " << o.spec << " , ";
        fout << "Diff : " << o.diff;
        return fout;
    }
}