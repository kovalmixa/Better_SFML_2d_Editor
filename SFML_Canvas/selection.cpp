#include "selection.h"

Selection* Selection::instance_ = nullptr;

Selection::Selection()
{
	transform_points_[ResizeMode::Angle] = new sf::ConvexShape(3);
	auto triangle = dynamic_cast<sf::ConvexShape*>(transform_points_[ResizeMode::Angle]);
	triangle->setPoint(0, { 0, 0 });
	triangle->setPoint(1, { TRANSFORM_POINTS_SIZE, 0 });
	triangle->setPoint(2, { TRANSFORM_POINTS_SIZE / 2, TRANSFORM_POINTS_SIZE });

	transform_points_[ResizeMode::Center] = new sf::RectangleShape({ TRANSFORM_POINTS_SIZE, TRANSFORM_POINTS_SIZE });
	transform_points_[ResizeMode::Center]->setRotation(sf::degrees(45.f));

	transform_points_[ResizeMode::TopLeft] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[ResizeMode::Left] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[ResizeMode::BottomLeft] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[ResizeMode::TopRight] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[ResizeMode::Right] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[ResizeMode::BottomRight] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[ResizeMode::Top] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[ResizeMode::Bottom] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
}

Selection::~Selection()
{
	for (auto transform_point : transform_points_)
		delete transform_point.second;
	transform_points_.clear();
}

void Selection::update_transform_points()
{
	float left = transform_data.position.x;
	float top = transform_data.position.y;
	float right = left + transform_data.size.x;
	float bottom = top + transform_data.size.y;

	float centerX = left + transform_data.size.x / 2.f;
	float centerY = top + transform_data.size.y / 2.f;

	for (auto transform_point : transform_points_) {
		auto shape = transform_point.second;
		switch (transform_point.first)
		{
		case ResizeMode::Angle: shape->setPosition({ centerX, top - transform_data.size.y / 2.f }); break;
		case ResizeMode::Center: shape->setPosition({ centerX, centerY }); break;
		case ResizeMode::TopLeft: shape->setPosition({ left, top }); break;
		case ResizeMode::Left: shape->setPosition({ left, centerY }); break;
		case ResizeMode::BottomLeft: shape->setPosition({ left, bottom }); break;
		case ResizeMode::TopRight: shape->setPosition({ right, top }); break;
		case ResizeMode::Right: shape->setPosition({ right, centerY }); break;
		case ResizeMode::BottomRight: shape->setPosition({ right, bottom }); break;
		case ResizeMode::Top: shape->setPosition({ centerX, top }); break;
		case ResizeMode::Bottom: shape->setPosition({ centerX, bottom }); break;
		default: break;
		}
	}
}

Selection* Selection::get_instance() { return (instance_ == nullptr) ? new Selection() : instance_; }

bool Selection::try_select_shape(BaseShape* shape, sf::Vector2f point)
{
	if (!shape->contains(point)) return false;
	try_add_figure_to_selection(shape);
	return true;
}

void Selection::try_add_figure_to_selection(BaseShape* shape)
{

}

void Selection::clear_selection()
{
}

void Selection::draw_selection(sf::RenderWindow& window)
{
	for (auto shape : selected_shapes)
		shape->set_outline(is_selected_);

	if (is_selected_) {
		for (auto transform_point : transform_points_)
			window.draw(*transform_point.second);
	}
}

void Selection::try_resize(sf::Vector2f mouse_position)
{
}

void Selection::resize_selected_shapes(sf::Vector2f mouse_position)
{
}

