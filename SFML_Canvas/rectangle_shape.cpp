#include "rectangle_shape.h"

void RectangleShape::apply_transform()
{
	shape_->setPosition(transform_data_.position);
	shape_->setRotation(transform_data_.rotation);
	dynamic_cast<sf::RectangleShape*>(shape_)->setSize(transform_data_.size);
}

RectangleShape::RectangleShape()
{
	shape_ = new sf::RectangleShape({ 1.f, 1.f });
	shape_->setFillColor(sf::Color::White);
	set_pivot_center();
}

bool RectangleShape::contains(sf::Vector2f point)
{
	return dynamic_cast<sf::RectangleShape*>(shape_)->getGlobalBounds().contains(point);
}

void RectangleShape::resize(sf::Vector2f delta)
{
}
