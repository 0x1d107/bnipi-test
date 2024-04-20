#pragma once
#include <cmath>
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
class Polygon : public Polyline {
	public:
	Polygon( Point pos, int n,double r=1){
		std::vector<Point> pts(n);
		for(int i=0;i<n;i++){
			pts[i] = {pos.X+std::cos(2*M_PI*i/n)*r,
						   pos.Y+std::sin(2*M_PI*i/n)*r};

		}
		pts.push_back(pts[0]);
		set_points(pts);
	}

};


