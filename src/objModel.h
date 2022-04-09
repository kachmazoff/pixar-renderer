#include <vector>
#include <cmath>
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

class Vector3D
{
private:
    float _x, _y, _z;

public:
    Vector3D(float x, float y, float z) : _x(x), _y(y), _z(z){};
    float x() const { return _x; }
    float y() const { return _y; }
    float z() const { return _z; }

    friend Vector3D operator+(Vector3D a, const Vector3D &b)
    {
        a._x += b._x;
        a._y += b._y;
        a._z += b._z;
        return a;
    }

    friend Vector3D operator-(Vector3D a, const Vector3D &b)
    {
        a._x -= b._x;
        a._y -= b._y;
        a._z -= b._z;
        return a;
    }

    float length() const
    {
        return sqrt(_x * _x + _y * _y + _z * _z);
    }
};

float operator^(const Vector3D &a, const Vector3D &b)
{
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

Vector3D operator*(const Vector3D &a, const Vector3D &b)
{
    float x = (a.y() * b.z() - a.z() * b.y());
    float y = (a.z() * b.x() - a.x() * b.z());
    float z = (a.x() * b.y() - a.y() * b.x());

    return move(Vector3D(x, y, z));
}

std::ostream &operator<<(std::ostream &out, const Vector3D &v)
{
    return out << "{" << v.x() << ", " << v.y() << ", " << v.z() << "}";
}

float angle(const Vector3D &a, const Vector3D &b)
{
    return (a ^ b) / (a.length() * b.length());
}

vector<vector<float> > mul(const vector<vector<float> >& a, const vector<vector<float> >& b) {
    vector<vector<float> > ans(a.size(), vector<float>(b[0].size(), 0));

    for (int y = 0; y < ans.size(); ++y) {
        for (int x = 0; x < ans[0].size(); ++x) {
            for (int q = 0; q < a[0].size(); ++q) {
                ans[y][x] += a[y][q] * b[q][x];
            }
        }
    }

    return ans;
}

vector<float> mul(const vector<vector<float> >& a, const Point& b) {
    vector<float> ans(3, 0);

    for (int y = 0; y < 3; ++y) {
        ans[y] = a[y][0] * b.x() + a[y][1] * b.y() + a[y][2] * b.z();
    }

    return ans;
}

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
    vector<Point> normals;

    // Find barycentric coords for given pixel
    vector<float> barycentric_coords(const int &px_x, const int &px_y, const vector<float> &x, const vector<float> &y)
    {
        vector<float> res;
        for (size_t i = 0; i < 3; i++)
        {
            float mulX = (x[(i + 1) % 3] - x[(i + 2) % 3]),
                  mulY = (y[(i + 1) % 3] - y[(i + 2) % 3]);

            float lambda =
                ((px_y - y[(i + 2) % 3]) * mulX - (px_x - x[(i + 2) % 3]) * mulY) /
                ((y[i] - y[(i + 2) % 3]) * mulX - (x[i] - x[(i + 2) % 3]) * mulY);

            res.push_back(lambda);
        }

        return move(res);
    }

    bool is_inside(const vector<float> &coords)
    {
        bool checkInside = true;
        for (size_t i = 0; i < coords.size() && checkInside; i++)
        {
            checkInside = !(coords[i] < 0);
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
            else if (skip == "vn") // Parse normals
            {
                float x, y, z;
                ss >> x >> y >> z;
                normals.push_back(Point(x, y, z));
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
        const float PI = 3.14;
        float angle_alpha = 0 , angle_beta = PI, angle_gamma = 0;

        vector<vector<float> > A(3, vector<float>(3, 0));
        A[0][0] = 1;
                        A[1][1] = cos(angle_alpha);     A[1][2] = sin(angle_alpha);
                        A[2][1] = -sin(angle_alpha);    A[2][2] = cos(angle_alpha);

        vector<vector<float> > B(3, vector<float>(3, 0));
        B[0][0] = cos(angle_beta);              B[0][2] = sin(angle_beta);
                                    B[1][1] = 1;
        B[2][0] = -sin(angle_beta);             B[2][2] = cos(angle_beta);

        vector<vector<float> > C(3, vector<float>(3, 0));
        C[0][0] = cos(angle_gamma);    C[0][1] = sin(angle_gamma);
        C[1][0] = -sin(angle_gamma);   C[1][1] = cos(angle_gamma);
                                                                    C[2][2] = 1;

        vector<vector<float> > R = mul(mul(A, B), C);

        float focus = 500;
        float dist = focus;

        for (size_t f = 0; f < faces.size(); ++f)
        {
            const vector<int> &pointNums = faces[f].getPointNums();
            vector<float> x, y, z;

            // Get face's points coords in arrays (for convenience)
            for (size_t p = 0; p < pointNums.size(); ++p)
            {
                vector<float> new_coords = mul(R, points[pointNums[p]]);

                float curr_z = new_coords[2] + dist;
                x.push_back(new_coords[0] * ZOOM / curr_z * focus + CAM_X_OFFSET);
                y.push_back(new_coords[1] * ZOOM / curr_z * focus + CAM_Y_OFFSET);
                z.push_back(curr_z * ZOOM);

                // float curr_z = points[pointNums[p]].z() + 1;
                // x.push_back(points[pointNums[p]].x() * ZOOM / curr_z + CAM_X_OFFSET);
                // y.push_back(points[pointNums[p]].y() * ZOOM / curr_z + CAM_Y_OFFSET);
                // z.push_back(curr_z * ZOOM);
            }

            // Find bounding box (with clamping according to image dimensions)
            float x_min = max(*min_element(x.begin(), x.end()), float(0));
            float x_max = min(*max_element(x.begin(), x.end()), float(image.width()));

            float y_min = max(*min_element(y.begin(), y.end()), float(0));
            float y_max = min(*max_element(y.begin(), y.end()), float(image.height()));

            Vector3D v0(x[0], y[0], z[0]);
            Vector3D v1(x[1], y[1], z[1]);
            Vector3D v2(x[2], y[2], z[2]);

            Vector3D res = (v1 - v0) * (v1 - v2);
            Vector3D cam(0, 0, 1);
            float ang = angle(res, cam);

            if (ang <= 0)
            {
                continue;
            }

            int q = int(ang * 255) & 255;
            uint curr_color = (q << 16 | q << 8 | q);

            // For every pixel in bb decide: draw or not
            for (size_t px_x = x_min; px_x < x_max; ++px_x)
            {
                for (size_t px_y = y_min; px_y < y_max; ++px_y)
                {
                    vector<float> coords = barycentric_coords(px_x, px_y, x, y);
                    if (is_inside(coords))
                    {
                        float curr_z = z[0] * coords[0] + z[1] * coords[1] + z[2] * coords[2];
                        image.set_pixel(px_x, px_y, curr_z * cam.z(), curr_color);
                    }
                }
            }
        }
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
