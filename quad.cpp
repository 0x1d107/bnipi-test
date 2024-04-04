#include "quad.hpp"
#include "objects.hpp"
bool point_inside_rect(double px,double py,double ax,double ay,double bx,double by){
	return ax<px && px<bx && ay<py && py<by;
}
QuadTree::QuadTree(){

}
void QuadTree::insert(Polyline *poly){

}
void QuadTree::get_inside_box(std::vector<Polyline *> &result,double ax,double ay,double bx,double by){
}
