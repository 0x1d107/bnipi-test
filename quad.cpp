#include "quad.hpp"
#include "objects.hpp"
#include <cfloat>
#include <vector>
#include <iostream>

Point Point::operator+(const Point &other){
	Point p = {X+other.X,Y+other.Y};
	return p;
}
Point Point::operator-(const Point &other){
	Point p = {X-other.X,Y-other.Y};
	return p;
}
double Point::operator*(const Point &other){
	return X*other.X+Y*other.Y;
}
double Point::skew(const Point &other){
	return X * other.Y - other.X * Y;
}
bool line_segments_intersect(Point a1,Point b1, Point a2, Point b2 ){

	Point d1 = b1 - a1;
	Point d2 = b2 - a2;
	Point da = d2 - a1;
	double s12 = d1.skew(d2);
	if(abs(s12) <=1e-9){
		if(abs(d1.skew(da))<=1e-9){
			return true;	
		}
		return false;
	}
	double t = da.skew(d2)/s12;
	double u = da.skew(d1)/s12;
	return 0<=t&&t<=1&&0<=u&&u<=1;

}

bool Rect::contains(Rect &other){
	return ax <= other.ax && ay <= other.ay && other.bx <= bx && other.by <= by;
}
bool Rect::contains(Polyline &line){
	for(auto &point:line.get_points()){
		if(!(ax<point.X&&point.X<bx&&ay<point.Y&&point.Y<by))
			return false;
	}
	return true;
}
bool Rect::overlaps(Rect &other){
	return ax < other.bx && bx >= other.ax && ay < other.by && by >= other.ay;
}

bool Rect::overlaps(Polyline &line){
	if (line.get_points().size()<=1)
		return false;
	auto points = line.get_points();
	for(int i=1;i< points.size();i++){
		Point C = points[i-1];
		Point D = points[i];
		if(ax<=C.X&&C.X<=bx && ay<=C.Y&&C.Y<=by){
			return true;
		}
		if(ax<=D.X&&D.X<=bx&&ay<=D.Y&&D.Y<=by){
			return true;
		}
		Point R[4] = {
			{ax,ay},
			{ax,by},
			{bx,by},
			{bx,ay}
		};
		for(int k=0;k<4;k++){
			Point R1 = R[k];
			Point R2 = R[(k+1)%4];
			if(line_segments_intersect(C,D,R1,R2))
				return true;
		}
	}


	return false;
}
Rect Rect::getAABB(Polyline &poly){
	double ax = DBL_MAX;
	double ay = DBL_MAX;
	double bx = -DBL_MAX;
	double by = -DBL_MAX;

	for(auto &point:poly.get_points()){
		if(point.X<ax)
			ax = point.X;
		if(point.Y<ay)
			ay = point.Y;
		if(point.X>bx)
			bx = point.X;
		if(point.Y>by)
			by = point.Y;
	}
	return {ax,ay,bx,by};
}
Rect Quad::get_node_bb(int i){
	double ax = bounding_box.ax;
	double ay = bounding_box.ay;
	double bx = bounding_box.bx;
	double by = bounding_box.by;
	double cx = (ax+bx)/2;
	double cy = (ay+by)/2;
	switch(i){
		case 0:
			return {ax,ay,cx,cy};
		case 1:
			return {ax,cy,cx,by};
		case 2:
			return {cx,cy,bx,by};
		case 3:
			return {cx,ay,bx,cy};
	}
	Rect r = {0,0,0,0};
	return r;
}
void Quad::insert(Polyline *poly,Rect bb){
	for(int i=0;i<4;i++){
		Rect nodebb = get_node_bb(i);
		if(nodebb.contains(bb)){
			if(!nodes[i]){
				nodes[i] = new Quad();
				nodes[i]->bounding_box = nodebb;
			}
			nodes[i]->insert(poly,bb);
			return;
		}
	}
	polylines.push_back(poly);
}
void Quad::search(std::vector<Polyline *> &result,Rect screen){
	std::cout <<"[Quad::search] ["<<bounding_box.ax<<","<<bounding_box.ay
		<<"x"<<bounding_box.bx<<","<<bounding_box.by<<"]"<<std::endl;
	for(auto l:polylines)
		if(screen.overlaps(*l))
			result.push_back(l);
	for(int i=0;i<4;i++){
		if(nodes[i]&& nodes[i]->bounding_box.overlaps(screen)){
			nodes[i]->search(result,screen);
			
		}
	}

	
}
void Quad::print(int depth){
	for(int i=0;i<depth;i++)
		std::cout << "  ";
	std::cout<<"["<<bounding_box.ax<<","<<bounding_box.ay<<" : " <<
		bounding_box.bx <<","<<bounding_box.by<<"] contains "<<polylines.size()<<" polylines"<<std::endl;
	for(int i=0;i<4;i++){
		if(nodes[i]){
			nodes[i]->print(depth+1);
		}else {
			for(int i=0;i<depth+1;i++)
				std::cout << "  ";
			std::cout<<"null"<<std::endl;
		}
	}
}
Quad::~Quad(){
	for(int i=0;i<4;i++)
		delete nodes[i];
}


QuadTree::QuadTree(double width,double height){
	Rect bb = {0,0,width,height};
	root.bounding_box = bb;
}
void QuadTree::insert(Polyline *poly){
	root.insert(poly,Rect::getAABB(*poly));

}
std::vector<Polyline *>  QuadTree::get_inside_box(Rect &rect){
	//root.print();
	std::vector<Polyline *> polylines;
	root.search(polylines,rect);
	return polylines;
}
QuadTree::~QuadTree(){
}
