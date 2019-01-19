#include <iostream>
#include "Canvas.h"
#include "Stage.h"
using namespace std;

int main(int argc, char **argv) {
    string name;
    if (argc == 2)
        name = argv[1];
    else
        name = "try.png";
    stage::Stage stg("input.txt");
    cout <<stg;
    Canvas *canvas = stg.ray_trace();
    canvas->draw_to_file(name);
    delete canvas;
    return 0;
}