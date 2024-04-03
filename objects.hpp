#pragma once
#include <vector>

struct Point{
	double X;
	double Y;
};
class Polyline {
public:
    void set_points(const std::vector<Point> aPoints)
    {
        _points = aPoints;
    }
    const std::vector<Point>& get_points()
    {
        return _points;
    }
private:
    std::vector<Point> _points;
};
class Polygon : public Polyline {};


// поиск по прямоугольной области экрана 
std::vector<Polyline> find_objects(std::pair<double, double> aLeftTop, 
									std::pair<double, double> aRightBottom);
