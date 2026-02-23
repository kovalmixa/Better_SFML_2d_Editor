#define _USE_MATH_DEFINES
#include <cmath>

#include "polygon_shape.h"

void PolygonShape::apply_transform()
{
    shape_->setPosition(transform_data_.position);
    shape_->setRotation(transform_data_.rotation);
    //dynamic_cast<sf::RectangleShape*>(shape_)->setSize(transform_data_.size);
}

void PolygonShape::setup_polygon_points()
{
    for (int i = 0; i < 6; ++i) {
        float angle = 2 * M_PI * i / 6;
        float x = transform_data_.size.x * std::cos(angle);
        float y = transform_data_.size.y * std::sin(angle);
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

bool PolygonShape::contains(sf::Vector2f point)
{
	return dynamic_cast<sf::ConvexShape*>(shape_)->getGlobalBounds().contains(point);
}

void PolygonShape::resize(sf::Vector2f delta)
{
}
