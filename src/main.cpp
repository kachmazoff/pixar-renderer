#include <iostream>
#include <cmath>
#include <vector>
#include "./Image.h"
#include "./liner.h"
#include "./objModel.h"

using namespace std;

void drawStar(Image &image, Liner &liner)
{
    int x0 = 100, y0 = x0;
    for (size_t i = 0; i < 12; ++i)
    {
        float alpha = 2 * M_PI * i / 13;
        int x1 = x0 + 95 * cos(alpha), y1 = y0 + 95 * sin(alpha);
        liner.draw(x0, y0, x1, y1, image);
    }
}

int main() {
    Image image(200);
    BresenhamLiner liner = BresenhamLiner();
    drawStar(image, liner);
    image.save("prikol.bmp");

    ObjModel model("fox.obj");
    Image result_image(200);

    const int CAM_X_OFFSET = 100;
    const int CAM_Y_OFFSET = 10;
    const int ZOOM = 2;

    auto start = std::chrono::system_clock::now();

    for (int f = 0; f < model.faces.size(); ++f) {
        for (int p = 0; p < model.faces[f].getPointNums().size(); ++p) {
            cout << "get\n";
            Point start_point = model.points[model.faces[f].getPointNums()[p]];
            Point end_point = model.points[model.faces[f].getPointNums()[(p + 1) % model.faces[f].getPointNums().size()]];
            cout << f << " " << start_point.x() << " " << start_point.y() << " " << end_point.x() << " " << end_point.y() << endl;
            // cout << f << endl;
            liner.draw(
                start_point.x() * ZOOM + CAM_X_OFFSET,
                start_point.y() * ZOOM + CAM_Y_OFFSET,
                end_point.x() * ZOOM + CAM_X_OFFSET,
                end_point.y() * ZOOM + CAM_Y_OFFSET,
                result_image
            );
            // result_image.save("render.bmp");
            // std::this_thread::sleep_for(std::chrono::milliseconds(60));
        }
    }
    result_image.save("render.bmp");

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Time: " << diff.count() << " s\n";

    return 0;
}
