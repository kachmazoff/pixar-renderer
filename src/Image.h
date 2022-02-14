#include "./bmp_writer.h"
#include "./colors.h"

class Image
{
private:
    vector<vector<uint> > _arr;
    int _width;
    int _height;

public:
    Image(int size) : _width(size), _height(size)
    {
        this->_arr.assign(size, vector<uint>(size));
    }

    Image(int width, int height)
    {
        this->_arr.assign(height, vector<uint>(width));
    }

    int width() const
    {
        return _width;
    }

    int height() const
    {
        return _height;
    }

    void set_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
    {
        if (x < 0 || x >= _width || y < 0 || y >= _height)
        {
            return;
        }
        uint res = (r << 16) | (g << 8) | b;
        // this->_arr[_height - y - 1][x] = res;
        this->_arr[y][x] = res;
    }

    void set_pixel(int x, int y, const color &c)
    {
        if (x < 0 || x >= _width || y < 0 || y >= _height)
        {
            return;
        }
        this->_arr[y][x] = c.value;
    }

    void save(string filename)
    {
        saveBitmapImage(this->_arr, &filename[0]);
    }
};
