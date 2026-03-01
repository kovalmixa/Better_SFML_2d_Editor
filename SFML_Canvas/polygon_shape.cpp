#define _USE_MATH_DEFINES
#include <cmath>

#include "polygon_shape.h"

void PolygonShape::apply_transform()
{
    shape_->setPosition(transform_data_.position);
    setup_polygon_points();
    shape_->setRotation(transform_data_.rotation);
}

void PolygonShape::setup_polygon_points()
{
    for (int i = 0; i < num_points_; ++i) {
        float angle = 2 * M_PI * i / num_points_ - (M_PI / 2);;
        float x = transform_data_.size.x * std::cos(angle);
        float y = transform_data_.size.y * std::sin(angle);
        dynamic_cast<sf::ConvexShape*>(shape_)->setPoint(i, sf::Vector2f(x, y));
    }
}

PolygonShape::PolygonShape(const int points_quantity)
{
    set_points(points_quantity);
}

void PolygonShape::set_points(const int points_quantity)
{
    delete shape_;
    if (points_quantity < 3) num_points_ = 3;
    else num_points_ = points_quantity;
    sf::ConvexShape* polygon = new sf::ConvexShape(points_quantity);
    shape_ = polygon;
    setup_polygon_points();
}

bool PolygonShape::contains(sf::Vector2f point)
{
    return dynamic_cast<sf::ConvexShape*>(shape_)->getGlobalBounds().contains(point); 
}

BaseShape* PolygonShape::clone()
{
    auto* copy = new PolygonShape();
    copy->set_transform(this->get_transform());
    copy->set_color(this->get_color());
    return copy;
}
