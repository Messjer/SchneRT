#include <iostream>
#include "Canvas.h"
#include "Stage.h"
using namespace std;

int main(int argc, char **argv) {
    double samp = 100, resl = 1;
    if (argc == 3) {
        samp = strtol(argv[1], NULL, 10);
        resl = strtod(argv[2], NULL);
    }
    stage::Stage stg;
    Canvas *canvas = stg.RayTrace(samp, resl);
    canvas -> drawToFile("try.ppm");
    delete canvas;
    return 0;
}