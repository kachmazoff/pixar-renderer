#include <iostream>
#include <cmath>
#include <vector>
#include <string>

#include "./Image.h"
#include "./liner.h"
#include "./objModel.h"
#include "./colors.h"

using namespace std;

int main(int argc, char **argv)
{

    Image image(1024);

    string filename = argv[1];
    ObjModel model("./obj/" + filename + ".obj");

    model.rasterize(image, 500, image.width() / 2, image.height() / 2);

    image.save("./res/rend_" + filename + ".bmp");

    return 0;
}
