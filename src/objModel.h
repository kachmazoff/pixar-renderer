#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class Point
{
private:
    float _x, _y, _z;

public:
    Point(float x, float y, float z) : _x(x), _y(y), _z(z){};
    float x() const { return _x; }
    float y() const { return _y; }
    float z() const { return _z; }
};

class Face
{
private:
    vector<int> _pointNums;

public:
    Face(vector<int> pointNums) : _pointNums(pointNums){};
    const vector<int> &getPointNums() { return _pointNums; }
};

class ObjModel
{
private:
    vector<Point> points;
    vector<Face> faces;

    bool isInside(const int &px_x,
                  const int &px_y,
                  const vector<float> &x,
                  const vector<float> &y)
    {
        bool checkInside = true;
        // Find barycentric coords for given pixel
        for (size_t i = 0; i < 3 && checkInside; i++)
        {
            float mulX = (x[(i + 1) % 3] - x[(i + 2) % 3]),
                  mulY = (y[(i + 1) % 3] - y[(i + 2) % 3]);

            float lambda =
                ((px_y - y[(i + 2) % 3]) * mulX - (px_x - x[(i + 2) % 3]) * mulY) /
                ((y[i] - y[(i + 2) % 3]) * mulX - (x[i] - x[(i + 2) % 3]) * mulY);

            if (lambda < 0)
                checkInside = false;
        }
        return checkInside;
    }

public:
    ObjModel(const string fileName)
    {
        ifstream fin;
        fin.open(fileName, ios::in);
        if (!fin)
        {
            return;
        }
        while (!fin.eof())
        {
            string line, skip;
            getline(fin, line);
            istringstream ss(line.c_str());
            ss >> skip;
            if (skip == "v") // Parse points
            {
                float x, y, z;
                ss >> x >> y >> z;
                points.push_back(Point(x, y, z));
            }
            else if (skip == "f") // Parse faces
            {
                int num;
                vector<int> pointNums;
                while (ss >> num)
                {
                    pointNums.push_back(--num);
                    ss >> skip;
                }
                faces.push_back(Face(pointNums));
            }
        }
        fin.close();
    }

    void rasterize(
        Image &image,
        const float &ZOOM = 1,
        const int &CAM_X_OFFSET = 0,
        const int &CAM_Y_OFFSET = 0)
    {
        for (size_t f = 0; f < faces.size(); ++f)
        {
            const vector<int> &pointNums = faces[f].getPointNums();
            vector<float> x, y;

            // Get face's points coords in arrays (for convenience)
            for (size_t p = 0; p < pointNums.size(); ++p)
            {
                x.push_back(points[pointNums[p]].x() * ZOOM + CAM_X_OFFSET);
                y.push_back(points[pointNums[p]].y() * ZOOM + CAM_Y_OFFSET);
            }

            // Find bounding box (with clamping according to image dimensions)
            float x_min = max(*min_element(x.begin(), x.end()), float(0)),
                  x_max = min(*max_element(x.begin(), x.end()), float(image.width())),

                  y_min = max(*min_element(y.begin(), y.end()), float(0)),
                  y_max = min(*max_element(y.begin(), y.end()), float(image.height()));

            /**
             * TODO delete random colors -> change to lightning && backface culling
             */
            srand(f);
            int r_rand = rand() % 255, g_rand = rand() % 255, b_rand = rand() % 255;

            // For every pixel in bb decide: draw or not
            for (size_t px_x = x_min; px_x < x_max; ++px_x)
            {
                for (size_t px_y = y_min; px_y < y_max; ++px_y)
                {
                    if (isInside(px_x, px_y, x, y))
                    {
                        image.set_pixel(px_x, px_y, color(r_rand, g_rand, b_rand));
                    }
                }
            }
        }
    }

    void wireframe(
        Image &image,
        const float &ZOOM = 1,
        const int &CAM_X_OFFSET = 0,
        const int &CAM_Y_OFFSET = 0)
    {
        std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

        BresenhamLiner liner;
        for (size_t f = 0; f < faces.size(); ++f)
        {
            const vector<int> &pointNums = faces[f].getPointNums();

            for (size_t p = 0; p < pointNums.size(); ++p)
            {
                uint start_point_index = pointNums[p];
                uint end_point_index = pointNums[(p + 1) % pointNums.size()];

                Point start_point = points[start_point_index];
                Point end_point = points[end_point_index];

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

    string stringify() // For debug
    {
        string out;
        out += "points: " + to_string(points.size()) + ", faces: " + to_string(faces.size()) + "\n";
        for (size_t i = 0; i < points.size(); i++)
        {
            out += "v " +
                   to_string(points[i].x()) + " " +
                   to_string(points[i].y()) + " " +
                   to_string(points[i].z()) + "\n";
        }
        for (size_t i = 0; i < faces.size(); i++)
        {
            vector<int> pointNums = faces[i].getPointNums();
            out += "f " +
                   to_string(pointNums[0]) + " " +
                   to_string(pointNums[1]) + " " +
                   to_string(pointNums[2]) + "\n";
        }
        return out;
    }
};
