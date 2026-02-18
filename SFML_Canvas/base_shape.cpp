#include "base_shape.h"

void BaseShape::setup_transform_points()
{

}

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

void BaseShape::set_transform(sf::Vector2f position, sf::Vector2f size, sf::Angle rotation)
{
	shape_->setPosition(position);
	shape_->setRotation(rotation);
	shape_->setScale(size);
}

void BaseShape::update_transform_points()
{
    auto bounds = shape_->getGlobalBounds();

    float left = bounds.position.x;
    float top = bounds.position.y;
    float right = left + bounds.size.x;
    float bottom = top + bounds.size.y;

    float centerX = left + bounds.size.x / 2.f;
    float centerY = top + bounds.size.y / 2.f;

    return {
        {left, top},          // TL
        {centerX, top},       // T
        {right, top},         // TR
        {right, centerY},     // R
        {right, bottom},      // BR
        {centerX, bottom},    // B
        {left, bottom},       // BL
        {left, centerY}       // L
    };
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

void BaseShape::set_pivot_center()
{
	auto local = shape_->getLocalBounds();
	shape_->setOrigin({ local.size.x / 2.f, local.size.y / 2.f });
}
