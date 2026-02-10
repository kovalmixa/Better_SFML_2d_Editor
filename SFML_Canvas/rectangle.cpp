#include "rectangle.h"

Rectangle::Rectangle()
{
}

void Rectangle::draw(sf::RenderWindow& window)
{
}

bool Rectangle::contains(sf::Vector2f point)
{
	return dynamic_cast<sf::ConvexShape*>(shape_)->getGlobalBounds().contains(point);
}

void Rectangle::resize(sf::Vector2f delta)
{
}
