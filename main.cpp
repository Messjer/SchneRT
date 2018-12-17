#include <iostream>
#include "Canvas.h"
#include "Stage.h"
using namespace std;

int main(int argc, char **argv) {
    double samp = 200, resl = 1;
    if (argc == 3) {
        samp = strtol(argv[1], NULL, 10);
        resl = strtod(argv[2], NULL);
    }
    stage::Stage stg; /*
    stage::Ray r1 = stg.eye.getRay(365, 753);
    auto rst1 = stg.intersect(r1);
    auto hit = stg.objects[rst1.first];
    auto poc = r1.dir * rst1.second + r1.src;
    unsigned short i = time(NULL);
    cout <<i <<endl;
    srand(i);rand();
    auto l = rand();
    unsigned short Xi[3] = {0,0,l};
    auto shadow = stage::Ray(poc, stg.randomCosHemi(hit -> normal(stage::Ray(poc, r1.dir)), Xi));
    cout <<stg.intersect(shadow).first <<endl; */
    Canvas *canvas = stg.RayTrace(samp, resl);
    canvas -> drawToFile("try.ppm");
    delete canvas;
    return 0;
}