#include "base_shape.h"
#include "basic_functions.h"


void BaseShape::set_pivot_center()
{
	auto local = shape_->getLocalBounds();
	shape_->setOrigin({ local.size.x / 2.f, local.size.y / 2.f });
}

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
	transform_data_.size = size;
	transform_data_.rotation = rotation;
}

void BaseShape::set_transform(TransformData transform_data) { transform_data_ = transform_data; }

TransformData BaseShape::get_transform() { return transform_data_; }

sf::Color BaseShape::get_color() { return shape_->getFillColor(); }

void BaseShape::set_color(sf::Color color) { shape_->setFillColor(color); }

void BaseShape::set_outline(bool show)
{
	shape_->setOutlineColor(get_inverted_color(shape_->getFillColor()));
	shape_->setOutlineThickness(show ? 1.f : 0.f);
}

void BaseShape::draw(sf::RenderWindow& window) { window.draw(*shape_); }
