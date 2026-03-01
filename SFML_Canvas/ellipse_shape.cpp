#include "ellipse_shape.h"
#include "basic_functions.h"

void EllipseShape::apply_transform()
{
    auto new_size = transform_data_.size;
    dynamic_cast<sf::CircleShape*>(shape_)->setRadius(new_size.x);
    shape_->setScale({ 1, new_size.y / new_size.x});
    set_pivot_center_to_shape(*shape_);
    shape_->setPosition(transform_data_.position);
    shape_->setRotation(transform_data_.rotation);
}

EllipseShape::EllipseShape()
{
    shape_ = new sf::CircleShape(1, POINTS_NUM);
}

bool EllipseShape::contains(sf::Vector2f point)
{
    return dynamic_cast<sf::CircleShape*>(shape_)->getGlobalBounds().contains(point);
}

BaseShape* EllipseShape::clone()
{ 
    auto* copy = new EllipseShape();
    copy->set_transform(this->get_transform());
    copy->set_color(this->get_color());
    return copy;
}
