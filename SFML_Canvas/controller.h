#pragma once
#include <deque>
#include <functional>
#include <map>
#include "base_shape.h"
#include "ui.h"

class Controller
{
private:
	static Controller* instance_;

	std::deque<BaseShape*> shapes_;
	std::list<BaseShape*> selected_shapes_;
	std::map<ButtonAction, std::function<void(sf::Vector2f)>> handlers_;


	Controller();
	~Controller();

	void spawn_ellipse(sf::Vector2f position);
	void spawn_polygon(sf::Vector2f position);
	void copy_color(sf::Vector2f position);
	void paint_figure(sf::Vector2f position);
	void spawn_rectangle(sf::Vector2f position);

	void try_select_shape(sf::Vector2f position);
public:
	static Controller* get_instance();
	void execute_action(ButtonAction action, sf::Vector2f mouse_position);
	void render_shapes(sf::RenderWindow& window);
};
