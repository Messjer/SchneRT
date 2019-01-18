#include <iostream>
#include "Canvas.h"
#include "Stage.h"
using namespace std;

int main(int argc, char **argv) {
    stage::Stage stg("input.txt");
    cout <<stg;
    Canvas *canvas = stg.ray_trace();
    canvas->draw_to_file("try.png");
    delete canvas;
    return 0;
}