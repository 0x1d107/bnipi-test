#pragma once
#include "objects.hpp"
struct Rect {
	double ax;
	double ay;
	double bx;
	double by;
	bool contains( Rect & other);
	bool contains( Polyline & line);
	bool overlaps( Rect & other);
	bool overlaps( Polyline & other);
	static Rect getAABB(Polyline &poly);
};
struct Quad{
	Quad *nodes[4]={0};
	Rect bounding_box;
	std::vector<Polyline *> polylines;
	Rect get_node_bb(int i);
	void insert(Polyline *poly,Rect bb);
	void search(std::vector<Polyline *> &result, Rect screen); 
	void print(int depth=0);
	~Quad();
};
class QuadTree {
	public:
	QuadTree(double width,double height);
	void insert(Polyline *poly);
	std::vector<Polyline *> get_inside_box( Rect &rect);
	virtual ~QuadTree();

	protected:

	Quad root;
};
