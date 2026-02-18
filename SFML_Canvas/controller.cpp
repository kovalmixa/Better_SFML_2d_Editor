#include "controller.h"
#include <iostream>
#include <SFML/System/Exception.hpp>
#include "ui.h"
#include "rectangle.h"
#include "polygon.h"
#include "ellipse.h"

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
		Rectangle* new_rectangle = new Rectangle();
		new_rectangle->set_transform(position, { 20, 20 }, sf::degrees(0.f));
		shapes_.push_back(new_rectangle);
	}
	catch (sf::Exception exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}

Controller* Controller::get_instance() { return instance_ ? instance_ : instance_ = new Controller(); }

void Controller::execute_action(ButtonAction action, sf::Vector2f mouse_position)
{
	auto it = handlers_.find(action);
	if (it != handlers_.end()) it->second(mouse_position);
	else printf("No handler for this action, number: %d\n", action);
}

void Controller::render_shapes(sf::RenderWindow& window)
{
	for (auto& shape : shapes_) shape->draw(window);
}
