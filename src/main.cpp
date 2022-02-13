#include <iostream>
#include <cmath>
#include <vector>
#include "./Image.h"
#include "./liner.h"

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

int main()
{
    Image image(200);
    BresenhamLiner liner = BresenhamLiner();
    drawStar(image, liner);
    image.save("prikol.bmp");
    return 0;
}
