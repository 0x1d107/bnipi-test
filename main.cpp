#include <iostream>
#include "objects.hpp"
#include "quad.hpp"

std::vector<Polyline> _data;


std::vector<Polyline> find_objects(std::pair<double, double> aLeftTop, std::pair<double, double> aRightBottom)
{

	return _data;
}
void generate_polygons(){
	int N = 100;
	srand(119047);
	for(int i=0;i<N;i++){
		double x = rand()%1000;
		double y = rand()%1000;
		Polygon pol({x,y},4);
		_data.push_back(pol);
	}

}


int main(){
	generate_polygons();
	QuadTree tree;
	for(auto &poly:_data){
		tree.insert(&poly);
	}
	Rect screen = {0.0,0.0,10.0,10.0};
	auto visible = tree.get_inside_box(screen);
	for(auto poly:visible){
		std::cout << "Polygon: ";
		for(auto point: poly->get_points())
			std::cout <<"(" <<point.X<<","<<point.Y<<") ";
		std::cout<<std::endl;
	}
	return 0;
}
