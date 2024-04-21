#pragma once
#include "SFML/System/Vector2.hpp"
#include "SFML/System/Vector2.hpp"
#include <cmath>
#include <vector>

struct Point{
	double X;
	double Y;
	Point operator-(const Point& other);
	Point operator+(const Point& other);
	double operator*(const Point& other);
	double skew(const Point& other);
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
class Polygon : public Polyline {
	public:
	Polygon( Point pos, int n,double r=1){
		std::vector<Point> pts(n);
		for(int i=0;i<n;i++){
			pts[i].X = pos.X+std::cos(2*M_PI*i/n)*r;
			pts[i].Y = pos.Y+std::sin(2*M_PI*i/n)*r;

		}
		pts.push_back(pts[0]);
		set_points(pts);
	}

};


