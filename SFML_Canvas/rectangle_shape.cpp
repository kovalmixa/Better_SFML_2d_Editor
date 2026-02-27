#include "basic_functions.h"
#include "rectangle_shape.h"

void RectangleShape::apply_transform()
{
    auto rect = dynamic_cast<sf::RectangleShape*>(shape_);
    rect->setSize(transform_data_.size);
    set_pivot_center_to_shape(*shape_);
    shape_->setPosition(transform_data_.position);
    shape_->setRotation(transform_data_.rotation);
}

RectangleShape::RectangleShape()
{
	shape_ = new sf::RectangleShape({ 1.f, 1.f });
	shape_->setFillColor(sf::Color::White);
}

bool RectangleShape::contains(sf::Vector2f point)
{ return dynamic_cast<sf::RectangleShape*>(shape_)->getGlobalBounds().contains(point); }

BaseShape* RectangleShape::clone()
{
	auto* copy = new RectangleShape();
	copy->set_transform(this->get_transform());
	copy->set_color(this->get_color());
	return copy;
}
