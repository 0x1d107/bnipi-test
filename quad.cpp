#include "quad.hpp"
#include "objects.hpp"
#include <cfloat>
#include <vector>
#include <iostream>

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
	//vertical checks
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
		//vertical intersection
		if(D.X!=C.X){
			double y0 = C.Y + (ax -C.X)*(D.Y-C.Y)/(D.X-C.X); 
			double y1 = C.Y + (bx -C.X)*(D.Y-C.Y)/(D.X-C.X); 
			if(ay<=y0&&y0<=by){
				return true;	
			}
			if(ay<=y1&&y1<=by){
				return true;	
			}
		}
		//horizontal intersection
		if(D.Y!=C.Y){
			double x0 = C.X + (ay - C.Y)*(D.X - C.X)/(D.Y-C.Y);
			double x1 = C.X + (by - C.Y)*(D.X - C.X)/(D.Y-C.Y);
			if(ax<=x0&&x0<=bx){
				return true;	
			}
			if(ax<=x1&&x1<=bx){
				return true;	
			}
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
