#include <iostream>
#include <algorithm>
#include <SFML/System/Exception.hpp>

#include "ui.h"
#include "controller.h"
#include "rectangle_shape.h"
#include "polygon_shape.h"
#include "ellipse_shape.h"

Controller* Controller::instance_ = nullptr;

Controller::Controller()
{
	handlers_ = {
		{ButtonAction::Ellipse,  [this](sf::Vector2f position) { spawn_ellipse(position); }},
		{ButtonAction::Paint,	 [this](sf::Vector2f position) { paint_figure(position); } },
		{ButtonAction::Pipette,  [this](sf::Vector2f position) { copy_color(position); }},
		{ButtonAction::Polygon,  [this](sf::Vector2f position) { spawn_polygon(position); } },
		{ButtonAction::Rectangle,[this](sf::Vector2f position) { spawn_rectangle(position); } }
	};
}

Controller::~Controller()
{
	for (auto shape : shapes_) delete shape;
	shapes_.clear();
}

void Controller::spawn_ellipse(sf::Vector2f position)
{
	try
	{

	}
	catch (sf::Exception exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}

void Controller::spawn_polygon(sf::Vector2f position)
{
	try
	{

	}
	catch (sf::Exception exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}

void Controller::copy_color(sf::Vector2f position)
{
	try
	{

	}
	catch (sf::Exception exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}

void Controller::paint_figure(sf::Vector2f position)
{
	try
	{

	}
	catch (sf::Exception exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}

void Controller::spawn_rectangle(sf::Vector2f position)
{
	try
	{
		RectangleShape* new_rectangle = new RectangleShape();
		new_rectangle->set_transform(position, { 20, 20 }, sf::degrees(0.f));
		shapes_.push_back(new_rectangle);
	}
	catch (sf::Exception exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}

void Controller::try_select_shape(sf::Vector2f position)
{
	if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))) {
		for (auto shape : selected_shapes_)	shape->switch_selection();
		selected_shapes_.clear();
	}

	for (auto it = shapes_.rbegin(); it != shapes_.rend(); ++it){
		if ((*it)->try_select(position)) {
			selected_shapes_.push_back(*it);
			break;
		}
	}
}

Controller* Controller::get_instance() { return instance_ ? instance_ : instance_ = new Controller(); }

void Controller::execute_action(ButtonAction action, sf::Vector2f mouse_position)
{
	auto it = handlers_.find(action);
	if (it != handlers_.end()) it->second(mouse_position);
	else try_select_shape(mouse_position);
}

void Controller::render_shapes(sf::RenderWindow& window)
{
	for (auto& shape : shapes_) {
		shape->draw(window);
	}
	// выделения фигур
}
