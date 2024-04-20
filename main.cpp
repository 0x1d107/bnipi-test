#include <iostream>
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/PrimitiveType.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "objects.hpp"
#include "quad.hpp"
#include <SFML/Graphics.hpp>

std::vector<Polyline> _data;


QuadTree tree;
std::vector<Polyline> find_objects(std::pair<double, double> aLeftTop, std::pair<double, double> aRightBottom)
{
	Rect screen = {aLeftTop.first,aLeftTop.second,aRightBottom.first,aRightBottom.second};
	auto visible = tree.get_inside_box(screen);
	std::vector<Polyline> result;
	for(Polyline *ptr: visible)
		result.push_back(*ptr);
	return result;
}
void generate_polygons(){
	int N = 100;
	srand(119047);
	for(int i=0;i<N;i++){
		double x = rand()%1000;
		double y = rand()%1000;
		Polygon pol({x,y},4,20);
		_data.push_back(pol);
	}

}

void print_visible(std::vector<Polyline *> &visible){
	std::cout<<"BEGIN VISIBLE: COUNT = "<<visible.size()<<std::endl;
	for(auto poly:visible){
		std::cout << "Polygon: ";
		auto points = poly->get_points();
		for(int i=0;i<points.size();i++){
			auto point = points[i];
			std::cout <<"(" <<point.X<<","<<point.Y<<") ";
		}
		std::cout<<std::endl;
	}
	std::cout<<"END VISIBLE"<<std::endl;

}

int main(){
	sf::RenderWindow window(sf::VideoMode(800,800),"Polygons");
	sf::Font font;
	sf::Text textVisible;
	
	generate_polygons();
	for(auto &poly:_data){
		tree.insert(&poly);
	}
	Rect screen = {0.0,0.0,100.0,100.0};
	double screen_delta = 10;
	auto visible = tree.get_inside_box(screen);
	window.setFramerateLimit(20);
	print_visible(visible);
	while(window.isOpen()){
		sf::Event evt;
		while(window.pollEvent(evt)){
			if(evt.type == sf::Event::Closed){
				window.close();
			}	

			if(evt.type == sf::Event::KeyPressed){
				switch(evt.key.code){
				case sf::Keyboard::Up:
					screen.ay-=screen_delta;
					screen.by-=screen_delta;
					visible = tree.get_inside_box(screen);
					print_visible(visible);
					break;
				case sf::Keyboard::Down:
					screen.ay+=screen_delta;
					screen.by+=screen_delta;
					visible = tree.get_inside_box(screen);
					print_visible(visible);
					break;
				case sf::Keyboard::Left:
					screen.ax-=screen_delta;
					screen.bx-=screen_delta;
					visible = tree.get_inside_box(screen);
					print_visible(visible);
					break;
				case sf::Keyboard::Right:
					screen.ax+=screen_delta;
					screen.bx+=screen_delta;
					visible = tree.get_inside_box(screen);
					print_visible(visible);
					break;
				default:;
				}
			}
		}
		window.clear(sf::Color::Black);
		for(auto poly:visible){
			auto points = poly->get_points();
			sf::VertexArray verts(sf::LineStrip,points.size());
			for(int i=0;i<points.size();i++){
				Point point = points[i];
				sf::Vector2u winSize = window.getSize();
				double scalex = winSize.x/(screen.bx - screen.ax);
				double scaley = winSize.y/(screen.by - screen.ay);
				verts[i].position.x = scalex*(point.X-screen.ax);
				verts[i].position.y =scaley*( point.Y-screen.ay);
				verts[i].color=sf::Color::White;
			}
			window.draw(verts);
		}

		window.display();
	}
	return 0;
}
