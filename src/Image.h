#include "./bmp_writer.h"
#include "./colors.h"

class Image
{
private:
    vector<vector<uint> > _arr;
    vector<vector<float> > z_buffer;
    int _width;
    int _height;

public:
    Image(int size) : _width(size), _height(size)
    {
        this->_arr.assign(size, vector<uint>(size));
        this->z_buffer.assign(size, vector<float>(size, 1e9));
    }

    Image(int width, int height)
    {
        this->_arr.assign(height, vector<uint>(width));
        this->z_buffer.assign(height, vector<float>(width, 1e9));
    }

    int width() const
    {
        return _width;
    }

    int height() const
    {
        return _height;
    }

    void set_pixel(int x, int y, float z, const uint &c) {
        if (x < 0 || x >= _width || y < 0 || y >= _height || z >= z_buffer[y][x])
        {
            return;
        }
        this->_arr[y][x] = c;
        z_buffer[y][x] = z;
    }

    void set_pixel(int x, int y, float z, unsigned char r, unsigned char g, unsigned char b) {
        uint res = (r << 16) | (g << 8) | b;
        set_pixel(x, y, z, res);
    }

    void set_pixel(int x, int y, float z, const color &c) {
        set_pixel(x, y, z, c.value);
    }


    void save(string filename)
    {
        saveBitmapImage(this->_arr, &filename[0]);
    }
};
