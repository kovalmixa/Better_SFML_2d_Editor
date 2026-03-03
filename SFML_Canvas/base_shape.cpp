#include "base_shape.h"
#include "basic_functions.h"
#include "particle_system.h"

BaseShape::BaseShape()
{
	particle_system_ = new ParticleSystem(this);
}

BaseShape::~BaseShape()
{
	delete particle_system_;
	delete shape_;

	particle_system_ = nullptr;
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

void BaseShape::set_effect(EmitterMode mode) 
{ 
	particle_system_->emit(mode); 
}

void BaseShape::set_outline(bool show)
{ 
	shape_->setOutlineColor(sf::Color::Blue);
	shape_->setOutlineThickness(show ? 2.f : 0.f);
}

void BaseShape::draw(sf::RenderWindow& window)
{ 
	particle_system_->draw(window);
	window.draw(*shape_); 
}
