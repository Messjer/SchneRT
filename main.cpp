#include <iostream>
#include "Canvas.h"
#include "Stage.h"
using namespace std;

int main() {
    stage::Stage stg;
    Canvas *canvas = stg.RayTrace();
    canvas -> drawToFile("try.ppm");
    delete canvas;
    return 0;
}