#define _USE_MATH_DEFINES
#include <cmath>

#include "polygon_shape.h"

void PolygonShape::setup_polygon_points()
{
    for (int i = 0; i < 6; ++i) {
        float angle = 2 * M_PI * i / 6;
        float x = width_ * std::cos(angle);
        float y = height_ * std::sin(angle);
        dynamic_cast<sf::ConvexShape*>(shape_)->
            setPoint(i, sf::Vector2f(x, y));
    }
}

PolygonShape::PolygonShape()
{
    sf::ConvexShape* polygon = new sf::ConvexShape();
	shape_ = polygon;
    setup_polygon_points();
}

void PolygonShape::set_points(const std::vector<sf::Vector2f>& points)
{
}

void PolygonShape::set_transform(sf::Vector2f position, sf::Vector2f size, sf::Angle rotation)
{
}

bool PolygonShape::contains(sf::Vector2f point)
{
	return dynamic_cast<sf::ConvexShape*>(shape_)->getGlobalBounds().contains(point);
}

void PolygonShape::resize(sf::Vector2f delta)
{
}
