//
// Created by Hong Man Hou on 13/1/2019.
//

#include "Bezier.h"

using namespace std;
using namespace stage;

Vec BezierCurve::eval(BezierCurve curve, double t) {
    // bezier curve evaluation using de Casteljau's algo
    if (curve.c_points.size() == 1)
        return curve.c_points[0];
    vector<Vec> new_points;
    int size = curve.c_points.size();
    for (int i = 0; i < size - 1; i++)
        new_points.push_back(curve.c_points[i] * (1 - t) + curve.c_points[i + 1] * t);
    BezierCurve reduced = BezierCurve(new_points);
    return eval(reduced, t);
}

Vec BezierCurve::deri(BezierCurve curve, double t) {
    if (curve.c_points.size() == 1)
        return curve.c_points[0];
    vector<Vec> new_points;
    int size = curve.c_points.size();
    for (int i = 0; i < size - 1 ; i++)
        new_points.push_back((curve.c_points[i + 1] - curve.c_points[i]) * (size - 1) * t);
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

Vec BezierRotational::eval(double u, double v) {
    BezierCurve curve = BezierCurve(points);
    Vec point = curve.eval(curve, u);
    point = point.rotate(axis, 2 * PI * v) + pos;
    return point;
}

void BezierRotational::compute_b_box() {
    double max_abs[3];
    for (int i = 0; i < points.size(); i++) {
        if (abs(points[i][0]) > max_abs[0])
            max_abs[0] = abs(points[i][0]);
        if (points[i][2] < b_box.low[2])
            b_box.low[2] = points[i][2];
        if (points[i][2] > b_box.high[2])
            b_box.high[2] = points[i][2];
    }
    b_box.low[0] = -max_abs[0];
    b_box.high[0] = max_abs[0];
    b_box.low[1] = -max_abs[0];
    b_box.high[1] = max_abs[0];
    for (int d = 0; d < 3; d++)
    {
        b_box.low[d] += pos[d];
        b_box.high[d] += pos[d];
    }
    b_box.make_faces();
}

Intersection BezierRotational::intersect(const Ray &ray) const {
    Intersection with_box = b_box.intersect(ray);
    if (with_box.type == MISS) return with_box;

    // only if with_box is not miss will we use Newton's method
    double t = with_box.t;
}

namespace stage {
    std::istream &operator>>(std::istream &fin, BezierRotational &b) {
        string str;
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
                b.points = upoints;
            }
            else if (str == "diff") fin >> b.diff;
            else if (str == "spec") fin >> b.spec;
            else if (str == "refr") fin >> b.refr;
            else if (str == "color") fin >> b.color;
            else if (str == "axis") fin >> b.axis;
            else {
                cerr <<string("Unrecognized field: ") + str + " for Bezier rotational!" <<endl;
                exit(-1);
            }
        }
        b.compute_b_box();
        b.b_box.make_faces();
        return fin;
    }

    std::ostream &operator<<(std::ostream &fout, const BezierRotational &o) {
        fout << "Beizer rotational at: " <<o.pos << endl;
        fout << "Control Points: " <<endl;
        for (auto pt : o.points) {
            fout << pt <<endl;
        }
        fout << "Axis : " <<o.axis;
        fout <<endl;
        fout << "Bounding box is " <<o.b_box <<endl;

        return fout;
    }
}