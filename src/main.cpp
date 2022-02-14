#include <iostream>
#include <cmath>
#include <vector>

#include "./Image.h"
#include "./liner.h"
#include "./objModel.h"
#include "./colors.h"

using namespace std;

void drawStar(Image &image, const Liner &liner)
{
    int x0 = image.width() / 2, y0 = image.height() / 2;
    for (size_t i = 0; i <= 12; ++i)
    {
        float alpha = 2 * M_PI * i / 13;

        int x1 = x0 + image.width() / 2 * 0.95 * cos(alpha),
            y1 = y0 + image.height() / 2 * 0.95 * sin(alpha);

        liner.draw(x0, y0, x1, y1, image);
    }
}

void render(
    Image &image,
    ObjModel &model,
    const Liner &liner,
    const int &CAM_X_OFFSET = 0,
    const int &CAM_Y_OFFSET = 0,
    const float &ZOOM = 1)
{
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    for (size_t f = 0; f < model.faces.size(); ++f)
    {
        const vector<int> &pointNums = model.faces[f].getPointNums();

        for (size_t p = 0; p < pointNums.size(); ++p)
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
                image);
        }
    }

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Time: " << diff.count() * 1000 << " ms\n";
}

int main()
{

    vector<Liner *> liners;
    liners.push_back(new SimpleLiner());
    liners.push_back(new RelativeLiner());
    liners.push_back(new SteepLiner());
    liners.push_back(new BresenhamLiner());

    for (size_t i = 0; i < liners.size(); i++)
    {
        Image image(512);
        drawStar(image, *liners[i]);
        image.save("./res/" + to_string(i) + "_star.bmp");
    }

    BresenhamLiner bresenhamLiner;

    Image image(512);
    string fileName = "monsta_triang";
    ObjModel model1(("./obj/" + fileName + ".obj").c_str());
    // ObjModel model2("./obj/storm_trooper_triang.obj");
    render(image, model1, bresenhamLiner, image.width() / 2, image.height() / 2, 500);
    // render(image, model2, bresenhamLiner, image.width() / 2, image.height() / 2 + 88, 363);
    image.save("./res/rend_" + fileName + ".bmp");

    return 0;
}
