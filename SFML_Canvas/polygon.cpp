#include "polygon.h"

Polygon::Polygon()
{
    auto polygon = new sf::ConvexShape();
    polygon->setPointCount(3);
    polygon->setPoint(0, { 0, 0 });
    polygon->setPoint(1, { 100, 0 });
    polygon->setPoint(2, { 50, 100 });
    shape_ = polygon;
}

void Polygon::draw(sf::RenderWindow& window)
{
	window.draw(*shape_);
}

void Polygon::set_transform(sf::Vector2f position, sf::Vector2f size, sf::Angle rotation)
{
}

bool Polygon::contains(sf::Vector2f point)
{
	return dynamic_cast<sf::ConvexShape*>(shape_)->getGlobalBounds().contains(point);
}

void Polygon::resize(sf::Vector2f delta)
{
}
