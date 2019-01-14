#include <iostream>
#include "Canvas.h"
#include "Stage.h"
using namespace std;

int main(int argc, char **argv) {
    double samp, resl;
    ifstream fin("render.config");
    fin >>samp >>resl;
    stage::Stage stg("stage.txt");
    cout <<stg;
    Canvas *canvas = stg.ray_trace(samp, resl);
    canvas->draw_to_file("try.png");
    delete canvas;
    return 0;
}