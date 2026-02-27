#include "ellipse_shape.h"

void EllipseShape::apply_transform()
{

}

EllipseShape::EllipseShape() : PolygonShape(16) { }

BaseShape* EllipseShape::clone() 
{ 
    auto* copy = new EllipseShape();
    copy->set_transform(this->get_transform());
    copy->set_color(this->get_color());
    return copy;
}
