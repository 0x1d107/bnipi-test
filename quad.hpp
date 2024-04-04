#pragma once
#include "objects.hpp"
class QuadTree {
	public:
	QuadTree();
	void insert(Polyline *poly);
	void get_inside_box(std::vector<Polyline *> &result,double ax,double ay,double bx,double by);
	virtual ~QuadTree();

	protected:

	struct Quad{
		Quad *nodes[4]={0};
		Quad *ur(){
			return nodes[0];
		}
		Quad *ul(){
			return nodes[1];
		}
		Quad *dr(){
			return nodes[2];
		}
		Quad *dl(){
			return nodes[3];
		}
		Point point;
		Polyline *poly;
	};
	Quad root;
};
