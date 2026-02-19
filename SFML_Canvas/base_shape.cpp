#include "base_shape.h"
#include "basic_functions.h"

void BaseShape::update_transform_points()
{
	auto bounds = shape_->getGlobalBounds();

	float left = bounds.position.x;
	float top = bounds.position.y;
	float right = left + bounds.size.x;
	float bottom = top + bounds.size.y;

	float centerX = left + bounds.size.x / 2.f;
	float centerY = top + bounds.size.y / 2.f;
	
	auto original_color = get_color();
	for (auto transform_point : transform_points_) {
		auto shape = transform_point.second;
		shape->setFillColor(get_inverted_color(original_color));
		switch (transform_point.first)
		{
		case ResizeMode::Angle: shape->setPosition({ centerX, top - bounds.size.y / 2.f }); break;
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

void BaseShape::set_pivot_center()
{
	auto local = shape_->getLocalBounds();
	shape_->setOrigin({ local.size.x / 2.f, local.size.y / 2.f });
}

BaseShape::BaseShape()
{
	transform_points_[ResizeMode::Angle] = new sf::ConvexShape(3);
	auto triangle = dynamic_cast<sf::ConvexShape*>(transform_points_[ResizeMode::Angle]);
	triangle->setPoint(0, { 0, 0 });
	triangle->setPoint(1, { TRANSFORM_POINTS_SIZE, 0 });
	triangle->setPoint(2, { TRANSFORM_POINTS_SIZE / 2, TRANSFORM_POINTS_SIZE });

	transform_points_[ResizeMode::Center] = new sf::RectangleShape({TRANSFORM_POINTS_SIZE, TRANSFORM_POINTS_SIZE});
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

BaseShape::~BaseShape()
{
	delete shape_;
	shape_ = nullptr;
	for (auto transform_point : transform_points_)
		delete transform_point.second;
	transform_points_.clear();
}

sf::Vector2f BaseShape::get_size() { return shape_->getGlobalBounds().size; }

sf::Vector2f BaseShape::get_position() { return shape_->getPosition(); }

sf::Color BaseShape::get_color() { return shape_->getFillColor(); }

void BaseShape::set_color(sf::Color color)
{
	shape_->setFillColor(color);
}

void BaseShape::draw(sf::RenderWindow& window)
{
	window.draw(*shape_);
	if (is_selected_) {
		shape_->setOutlineColor(get_inverted_color(shape_->getFillColor()));
		for(auto transform_point : transform_points_)
			window.draw(*transform_point.second);
	}
}

bool BaseShape::try_select(sf::Vector2f point)
{
	if (!contains(point)) return false;
	switch_selection();
	return true;
}

void BaseShape::switch_selection(){ is_selected_ = !is_selected_; }
