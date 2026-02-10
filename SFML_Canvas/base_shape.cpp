#include "base_shape.h"

BaseShape::BaseShape()
{
}

BaseShape::~BaseShape()
{
	delete shape_;
	delete hovered_shape_;
	shape_ = nullptr;
	hovered_shape_ = nullptr;
}

void BaseShape::set_transform(sf::Vector2f position, sf::Vector2f size, float rotation)
{
	transform_ = { position, size, rotation };
}

void BaseShape::set_color(sf::Color color)
{
	shape_->setFillColor(color);
}

void BaseShape::try_select(sf::Vector2f point)
{
	if (!contains(point)) return;
	is_selected_ = !is_selected_;
}
