#include <iostream>
#include <cmath>
#include <vector>

#include "./Image.h"
#include "./liner.h"
#include "./objModel.h"
#include "./colors.h"

using namespace std;

void drawStar(Image &image, Liner &liner)
{
    int x0 = image.width() / 2, y0 = image.height() / 2;
    for (size_t i = 0; i < 12; ++i)
    {
        float alpha = 2 * M_PI * i / 13;

        int x1 = x0 + image.width() / 2 * 0.95 * cos(alpha),
            y1 = y0 + image.height() / 2 * 0.95 * sin(alpha);

        liner.draw(x0, y0, x1, y1, image);
    }
}

int main()
{
    Image image(200);
    BresenhamLiner liner = BresenhamLiner();
    drawStar(image, liner);
    image.save("prikol.bmp");

    ObjModel model("obj/human_triang.obj");
    Image result_image(512);

    const int CAM_X_OFFSET = 512 / 2;
    const int CAM_Y_OFFSET = 512 / 2;
    const int ZOOM = 1000;

    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    for (size_t f = 0; f < model.faces.size(); ++f)
    {
        const vector<int> &pointNums = model.faces[f].getPointNums();

        for (size_t p = 0; p < model.faces[f].getPointNums().size(); ++p)
        {
            uint start_point_index = pointNums[p];
            uint end_point_index = pointNums[(p + 1) % pointNums.size()];

            Point start_point = model.points[start_point_index];
            Point end_point = model.points[end_point_index];

            liner.draw(
                start_point.x() * ZOOM + CAM_X_OFFSET,
                start_point.y() * ZOOM + CAM_Y_OFFSET,
                end_point.x() * ZOOM + CAM_X_OFFSET,
                end_point.y() * ZOOM + CAM_Y_OFFSET,
                result_image);
        }
    }

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Time: " << diff.count() * 1000 << " ms\n";

    result_image.save("render.bmp");

    return 0;
}
