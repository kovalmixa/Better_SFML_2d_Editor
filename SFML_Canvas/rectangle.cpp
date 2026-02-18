#include "rectangle.h"

Rectangle::Rectangle()
{
	shape_ = new sf::RectangleShape({ 1.f, 1.f });
	shape_->setFillColor(sf::Color::White);
	set_pivot_center();
}

void Rectangle::draw(sf::RenderWindow& window)
{
	window.draw(*shape_);
}

bool Rectangle::contains(sf::Vector2f point)
{
	return dynamic_cast<sf::RectangleShape*>(shape_)->getGlobalBounds().contains(point);
}

void Rectangle::resize(sf::Vector2f delta)
{
}
