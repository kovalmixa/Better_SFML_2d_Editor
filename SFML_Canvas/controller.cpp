#include "controller.h"
#include <iostream>
#include <SFML/System/Exception.hpp>
#include "ui.h"

Controller* Controller::instance_ = nullptr;

Controller::Controller()
{
	handlers_ = {
		{ButtonAction::Ellipse,  [this] { spawn_ellipse(); }},
		{ButtonAction::Paint,	 [this] { paint_figure(); } },
		{ButtonAction::Pipette,  [this] { copy_color(); }},
		{ButtonAction::Polygon,  [this] { spawn_polygon(); } },
		{ButtonAction::Rectangle,[this] { spawn_rectangle(); } }
	};
}

Controller::~Controller()
{
	for (auto shape : shapes_) delete shape;
	shapes_.clear();
}

void Controller::spawn_ellipse()
{
	try
	{

	}
	catch (sf::Exception exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}

void Controller::spawn_polygon()
{
	try
	{

	}
	catch (sf::Exception exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}

void Controller::copy_color()
{
	try
	{

	}
	catch (sf::Exception exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}

void Controller::paint_figure()
{
	try
	{

	}
	catch (sf::Exception exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}

void Controller::spawn_rectangle()
{
	try
	{
		std::cout << "spawn rectangle\n";
	}
	catch (sf::Exception exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}

Controller* Controller::get_instance() { return instance_ ? instance_ : instance_ = new Controller(); }

void Controller::execute_action(ButtonAction action)
{
	auto it = handlers_.find(action);
	if (it != handlers_.end()) it->second();
	else printf("No handler for this action, number: %d\n", action);
}
