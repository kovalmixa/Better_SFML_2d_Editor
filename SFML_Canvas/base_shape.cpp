#include "base_shape.h"
#include "basic_functions.h"

BaseShape::BaseShape()
{
}

BaseShape::~BaseShape()
{
	delete shape_;
	shape_ = nullptr;
}

void BaseShape::set_transform(sf::Vector2f position, sf::Vector2f size, sf::Angle rotation)
{
	transform_data_.position = position;
	transform_data_.size = { std::max(0.f, size.x), std::max(0.f, size.y) };
	transform_data_.rotation = rotation;
	apply_transform();
}

void BaseShape::set_transform(TransformData transform_data) 
{
	transform_data_ = transform_data;
	apply_transform();
}

TransformData BaseShape::get_transform() 
{
	return transform_data_; 
}

sf::FloatRect BaseShape::get_global_bounds() 
{ 
	return shape_->getGlobalBounds(); 
}

sf::Color BaseShape::get_color() 
{ 
	return shape_->getFillColor();
}

void BaseShape::set_color(sf::Color color)
{
	shape_->setFillColor(color);
}

void BaseShape::set_outline(bool show)
{ 
	shape_->setOutlineColor(sf::Color::Blue);
	shape_->setOutlineThickness(show ? 2.f : 0.f);
}

void BaseShape::draw(sf::RenderWindow& window)
{ 
	window.draw(*shape_); 
}
