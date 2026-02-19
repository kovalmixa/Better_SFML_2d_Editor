#include "rectangle_shape.h"

RectangleShape::RectangleShape()
{
	shape_ = new sf::RectangleShape({ 1.f, 1.f });
	shape_->setFillColor(sf::Color::White);
	set_pivot_center();
}

void RectangleShape::set_transform(sf::Vector2f position, sf::Vector2f size, sf::Angle rotation)
{
	shape_->setPosition(position);
	shape_->setRotation(rotation);
	shape_->setScale(size);
	update_transform_points();
}

bool RectangleShape::contains(sf::Vector2f point)
{
	return dynamic_cast<sf::RectangleShape*>(shape_)->getGlobalBounds().contains(point);
}

void RectangleShape::resize(sf::Vector2f delta)
{
}
