#include <iostream>
#include <vector>
#include "./Image.h"

using namespace std;

int main () {
    Image img(512);
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            unsigned char q = (unsigned char) ((x + y) % 256);
            img.set_pixel(x, y, q, q, q);
        }
    }
    img.save("chpok.bmp");


    for (int step = 0; step < 0; ++step) {

        Image img(128  + step * 2);
        for (int y = 0; y < img.height(); ++y) {
            for (int x = 0; x < img.width(); ++x) {
                unsigned char q = (unsigned char) ((x + y) % 256);
                img.set_pixel(x, y, q, q, q);
            }
        }
        img.save("chpok.bmp");

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
