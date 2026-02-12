#pragma once
#include <deque>
#include <functional>
#include <map>

#include "base_shape.h"
#include "ui.h"

class Controller
{
private:
	std::deque<BaseShape*> shapes_;
	static Controller* instance_;
	std::map<ButtonAction, std::function<void()>> handlers_;


	Controller();
	~Controller();

	void spawn_ellipse();
	void spawn_polygon();
	void copy_color();
	void paint_figure();
	void spawn_rectangle();
public:
	static Controller* get_instance();
	void execute_action(ButtonAction action);
};
