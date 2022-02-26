#include <iostream>
#include <cmath>
#include <vector>
#include <string>

#include "./Image.h"
#include "./objModel.h"
#include "./colors.h"

using namespace std;

int main(int argc, char **argv) {
    Image image(1024);

    // string filename = argv[1];
    string filename = "prikol18+";
    // string filename = "human_triang";
    // string filename = "storm_trooper_triang";
    ObjModel model("./obj/" + filename + ".obj");

    model.rasterize(image, 2500, image.width() / 2, image.height() / 2 - 1220);

    image.save("./res/rend_" + filename + ".bmp");
    return 0;
}
