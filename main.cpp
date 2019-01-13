#include <iostream>
#include "Canvas.h"
#include "Stage.h"
using namespace std;

int main(int argc, char **argv) {
    double samp = 20, resl = 1;
    if (argc == 3) {
        samp = strtol(argv[1], NULL, 10);
        resl = strtod(argv[2], NULL);
    }
    stage::Stage stg("stage.txt");
    cout <<stg;
    Canvas *canvas = stg.ray_trace(samp, resl);
    canvas->draw_to_file("try.png");
    delete canvas;
    return 0;
}