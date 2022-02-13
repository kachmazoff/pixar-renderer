#include <iostream>
using namespace std;

class Liner
{
public:
    virtual void draw(int x0, int y0, int x1, int y1, Image &image) = 0;
};

class SimpleLiner : public Liner
{
private:
    float _delta;

public:
    SimpleLiner() : _delta(.01) {}
    SimpleLiner(float delta) : _delta(delta) {}

    void draw(int x0, int y0, int x1, int y1, Image &image)
    {
        for (float t = 0.; t < 1.; t += _delta)
        {
            int x = x0 * (1. - t) + x1 * t;
            int y = y0 * (1. - t) + y1 * t;
            image.set_pixel(x, y, 255, 255, 255);
        }
    }
};

class RelativeLiner : public Liner
{
    void draw(int x0, int y0, int x1, int y1, Image &image)
    {
        for (size_t x = x0; x <= x1; ++x)
        {
            float t = float(x - x0) / (x1 - x0);
            int y = y0 * (1. - t) + y1 * t;
            image.set_pixel(x, y, 255, 255, 255);
        }
    }
};

class SteepLiner : public Liner
{
public:
    void draw(int x0, int y0, int x1, int y1, Image &image)
    {
        bool steep = abs(x0 - x1) < abs(y0 - y1);

        if (steep)
        {
            swap(x0, y0);
            swap(x1, y1);
        }
        if (x0 > x1)
        {
            swap(x0, x1);
            swap(y0, y1);
        }

        for (size_t x = x0; x <= x1; ++x)
        {
            float t = float(x - x0) / (x1 - x0);
            int y = y0 * (1. - t) + y1 * t;
            steep ? image.set_pixel(y, x, 255, 255, 255) : image.set_pixel(x, y, 255, 255, 255);
        }
    }
};