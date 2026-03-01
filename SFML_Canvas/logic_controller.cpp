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

void LogicController::spawn_rectangle(sf::Vector2f position)
{
	try
	{
		RectangleShape* new_rectangle = new RectangleShape();
		new_rectangle->set_transform(position, { SHAPE_SPAWN_SIZE, SHAPE_SPAWN_SIZE }, sf::degrees(0.f));
		new_rectangle->set_color(UIController::get_instance()->get_current_color());
		shapes_.push_back(new_rectangle);
	}
	catch (sf::Exception exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}

void LogicController::spawn_ellipse(sf::Vector2f position)
{
	try
	{
		EllipseShape* new_ellipse = new EllipseShape();
		new_ellipse->set_transform(position, { SHAPE_SPAWN_SIZE / 2, SHAPE_SPAWN_SIZE / 2 }, sf::degrees(0.f));
		new_ellipse->set_color(UIController::get_instance()->get_current_color());
		shapes_.push_back(new_ellipse);
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
		PolygonShape* new_polygon = new PolygonShape(UIController::get_instance()->get_polygon_points());
		new_polygon->set_transform(position, { SHAPE_SPAWN_SIZE / 2, SHAPE_SPAWN_SIZE / 2 }, sf::degrees(0.f));
		new_polygon->set_color(UIController::get_instance()->get_current_color());
		shapes_.push_back(new_polygon);
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
		UIController::get_instance()->set_button_action(ButtonAction::None);
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

void LogicController::move_figures(bool is_forward)
{
	auto selected_shapes = SelectionController::get_instance()->get_selected_shapes();
	if (selected_shapes.empty() || shapes_.size() < 2) return;

	if (is_forward) {
		for (auto it = std::prev(shapes_.end()); ; --it) {
			auto current = it;
			if (std::find(selected_shapes.begin(), selected_shapes.end(), *current) != selected_shapes.end()) {
				auto target = std::next(current);
				if (target != shapes_.end()) {
					std::iter_swap(current, target);
				}
			}
			if (it == shapes_.begin()) break;
		}
	}
	else {
		for (auto it = shapes_.begin(); it != shapes_.end(); ++it) {
			auto current = it;
			if (std::find(selected_shapes.begin(), selected_shapes.end(), *current) != selected_shapes.end()) {
				if (current != shapes_.begin()) {
					auto target = std::prev(current);
					std::iter_swap(current, target);
				}
			}
		}
	}
}

void LogicController::try_find_shape_to_select(sf::Vector2f position)
{
	bool ctrl_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);
	auto selection = SelectionController::get_instance();
	for (auto it = shapes_.rbegin(); it != shapes_.rend(); ++it)
		if (selection->try_select_shape(*it, position, ctrl_pressed)) return;
	selection->clear_selection();
}

LogicController* LogicController::get_instance() { return instance_ ? instance_ : instance_ = new LogicController(); }

void LogicController::execute_action(ButtonAction action, sf::Vector2f mouse_position)
{
	if (is_dragging_) return;
	auto it = handlers_.find(action);
	if (it != handlers_.end()) it->second(mouse_position);
	else try_find_shape_to_select(mouse_position);
}

void LogicController::keyboard_action_process(sf::Event event, sf::Vector2f mouse_position)
{
	if (!event.is<sf::Event::KeyPressed>()) return;
	const auto* key = event.getIf<sf::Event::KeyPressed>();
	auto selection_controller = SelectionController::get_instance();

	switch (key->code) {
		case sf::Keyboard::Key::Delete:
		case sf::Keyboard::Key::Backspace:
		{
			selection_controller->delete_selected_shapes();
			break;
		}
		case sf::Keyboard::Key::W:
		{
			move_figures(true);
			break;
		}
		case sf::Keyboard::Key::S:
		{
			move_figures(false);
			break;
		}
	}
	if (key->control)
	{
		switch (key->code)
		{
			case sf::Keyboard::Key::X:
			{
				selection_controller->delete_selected_shapes();
			}
			case sf::Keyboard::Key::C:
			{ 
				selection_controller->try_copy_shapes(); 
				break; 
			}
			case sf::Keyboard::Key::V:
			{ 
				selection_controller->try_paste_shapes(mouse_position); 
				break;
			}
			case sf::Keyboard::Key::A:
			{
				for (auto shape : shapes_) selection_controller->try_add_shape_to_selection(shape, true);
			}
		}
	}
}

void LogicController::begin_drag(sf::Vector2f mouse_position)
{
	if (is_dragging_) return;
	is_dragging_ = true;
	SelectionController::get_instance()->try_begin_drag_transform_mode(mouse_position);
}

void LogicController::end_drag()
{
	is_dragging_ = false;
	SelectionController::get_instance()->end_drag_transform_mode();
}

void LogicController::update_drag(ButtonAction action, sf::Vector2f mouse_position)
{
	if (!is_dragging_) return;
	auto selection = SelectionController::get_instance();
	switch (action)
	{
	case ButtonAction::None : selection->update_transform(mouse_position); break;
	case ButtonAction::Ellipse:
		break;
	case ButtonAction::Rectangle:
		break;
	case ButtonAction::Polygon:
		break;
	case ButtonAction::Pipette:
		break;
	case ButtonAction::Paint:
		break;
	default:
		break;
	}
}

void LogicController::remove_actions()
{
	UIController::get_instance()->set_button_action(ButtonAction::None);
	SelectionController::get_instance()->clear_selection();
}

void LogicController::add_shape(BaseShape* shape) { shapes_.push_back(shape);  }

void LogicController::delete_shape(BaseShape*& shape)
{
	if (!shape) return;
	SelectionController::get_instance()->remove_shape_from_selection(shape);
	auto found = std::find(shapes_.begin(), shapes_.end(), shape);
	if (found == shapes_.end()) {
		shape = nullptr;
		return;
	}
	shapes_.erase(found);
	delete shape;
	shape = nullptr;
	end_drag();
}

void LogicController::render_shapes(sf::RenderWindow& window)
{
	for (auto& shape : shapes_) shape->draw(window);
	SelectionController::get_instance()->draw_selection(window);
}
