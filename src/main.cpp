#include <iostream>
#include <cmath>
#include <vector>
#include <string>

#include "./Image.h"
#include "./objModel.h"
#include "./colors.h"

using namespace std;

int main(int argc, char **argv)
{
    string filename = argv[1];
    ObjModel model("./obj/" + filename + ".obj");

    // model.rasterize(image, 800, image.width() / 2, image.height() / 2);
    float zoom = 1./4;
    for (double angle = 0; angle < 2 * 3.14; angle += 0.1) {
        Image image(1024 * zoom);
        model.rasterize(image, 800 * zoom, image.width() / 2, image.height() / 2, angle);
        image.save("./res/rend_" + filename + ".bmp");
    }
    // model.rasterize(image, 800, image.width() / 2, image.height() / 2);

    // image.save("./res/rend_" + filename + ".bmp");
    return 0;
}
