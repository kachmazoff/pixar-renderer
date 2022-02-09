#include <iostream>
#include <vector>
#include "./bmp_writer.h"

using namespace std;

class Image {
private:
    vector<vector<uint> > _arr;
    int _width;
    int _height;
public:
    Image(int size): _width(size), _height(size) {
        this->_arr.assign(size, vector<uint>(size));
    }

    Image(int width, int height) {
        this->_arr.assign(height, vector<uint>(width));
    }

    int width() const {
        return _width;
    }

    int height() const {
        return _height;
    }

    void set_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
        uint res = (r << 16) | (g << 8) | b;
        this->_arr[_height - y - 1][x] = res;
    }

    void save(string filename) {
        saveBitmapImage(this->_arr, &filename[0]);
    }
};

int main () {
    Image img(256);
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            unsigned char q = (unsigned char) ((x + y) % 256);
            img.set_pixel(x, y, q, q, q);
        }
    }
    img.save("chpok.bmp");


    for (int step = 0; step < 1; ++step) {

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
