#include <iostream>
#include <algorithm>
#include <SFML/System/Exception.hpp>
#include <SFML/Graphics.hpp>

#include "ui_controller.h"
#include "logic_controller.h"
#include "selection_controller.h"
#include "rectangle_shape.h"
#include "polygon_shape.h"
#include "ellipse_shape.h"

LogicController* LogicController::instance_ = nullptr;

LogicController::LogicController()
{
	handlers_ = {
		{ButtonAction::Ellipse,  [this](sf::Vector2f position) { spawn_ellipse(position); }},
		{ButtonAction::Paint,	 [this](sf::Vector2f position) { paint_figure(position); } },
		{ButtonAction::Pipette,  [this](sf::Vector2f position) { copy_color(position); }},
		{ButtonAction::Polygon,  [this](sf::Vector2f position) { spawn_polygon(position); } },
		{ButtonAction::Rectangle,[this](sf::Vector2f position) { spawn_rectangle(position); } }
	};
}

LogicController::~LogicController()
{
	for (auto shape : shapes_) delete shape;
	shapes_.clear();
}

void LogicController::spawn_ellipse(sf::Vector2f position)
{
	try
	{

	}
	catch (sf::Exception exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}

void LogicController::spawn_polygon(sf::Vector2f position)
{
	try
	{

	}
	catch (sf::Exception exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}

void LogicController::copy_color(sf::Vector2f position)
{
	try
	{
		for (auto it = shapes_.rbegin(); it != shapes_.rend(); ++it) {
			if ((*it)->contains(position)) {
				(*it)->get_color();
				UIController::get_instance()->set_current_color((*it)->get_color());
				break;
			}
		}
		UIController::get_instance()->current_action = ButtonAction::None;
	}
	catch (sf::Exception exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}

void LogicController::paint_figure(sf::Vector2f position)
{
	try
	{
		//if (selected) selected->set_color(color);
		for (auto it = shapes_.rbegin(); it != shapes_.rend(); ++it) {
			if ((*it)->contains(position)) {
				(*it)->set_color(UIController::get_instance()->get_current_color());
				break;
			}
		}
	}
	catch (sf::Exception exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}

void LogicController::spawn_rectangle(sf::Vector2f position)
{
	try
	{
		RectangleShape* new_rectangle = new RectangleShape();
		new_rectangle->set_transform(position, { 20, 20 }, sf::degrees(0.f));
		new_rectangle->set_color(UIController::get_instance()->get_current_color());
		shapes_.push_back(new_rectangle);
	}
	catch (sf::Exception exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}

void LogicController::try_find_shape_to_select(sf::Vector2f position)
{
	bool ctrl_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);
	auto selection = Selection::get_instance();
	for (auto it = shapes_.rbegin(); it != shapes_.rend(); ++it)
		if (selection->try_select_shape(*it, position, ctrl_pressed)) return;
	selection->clear_selection();
}

LogicController* LogicController::get_instance() { return instance_ ? instance_ : instance_ = new LogicController(); }

void LogicController::execute_action(ButtonAction action, sf::Vector2f mouse_position)
{
	auto it = handlers_.find(action);
	if (it != handlers_.end()) it->second(mouse_position);
	else try_find_shape_to_select(mouse_position);
}

void LogicController::remove_actions()
{
	UIController::get_instance()->current_action = ButtonAction::None;
	Selection::get_instance()->clear_selection();
}

void LogicController::render_shapes(sf::RenderWindow& window)
{
	for (auto& shape : shapes_) shape->draw(window);
	Selection::get_instance()->draw_selection(window);
}
