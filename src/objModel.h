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
    const vector<int>& getPointNums() { return _pointNums; }
};

class ObjModel
{
public:
    vector<Point> points;
    vector<Face> faces;
    ObjModel(const char *fileName)
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
