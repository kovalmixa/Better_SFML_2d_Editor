#define _USE_MATH_DEFINES
#include <cmath>

#include "polygon_shape.h"

void PolygonShape::apply_transform()
{
    shape_->setPosition(transform_data_.position);
    set_size(transform_data_.size);
    //dynamic_cast<sf::RectangleShape*>(shape_)->setSize(transform_data_.size);
    shape_->setRotation(transform_data_.rotation);
}

void PolygonShape::setup_polygon_points(int points_quantity)
{
    for (int i = 0; i < points_quantity; ++i) {
        float angle = 2 * M_PI * i / points_quantity;
        float x = transform_data_.size.x * std::cos(angle);
        float y = transform_data_.size.y * std::sin(angle);
        dynamic_cast<sf::ConvexShape*>(shape_)->
            setPoint(i, sf::Vector2f(x, y));
    }
}

PolygonShape::PolygonShape(int points_quantity)
{
    if (points_quantity >= 3) points_quantity = num_points_;
    sf::ConvexShape* polygon = new sf::ConvexShape();
	shape_ = polygon;
    setup_polygon_points(points_quantity);
}

void PolygonShape::set_points(const std::vector<sf::Vector2f>& points)
{
}

bool PolygonShape::contains(sf::Vector2f point)
{ return dynamic_cast<sf::ConvexShape*>(shape_)->getGlobalBounds().contains(point); }

BaseShape* PolygonShape::clone()
{
    auto* copy = new PolygonShape();
    copy->set_transform(this->get_transform());
    copy->set_color(this->get_color());
    return copy;
}
