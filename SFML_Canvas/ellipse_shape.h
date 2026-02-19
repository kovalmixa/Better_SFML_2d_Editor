#pragma once
#include "base_shape.h"
#include "polygon_shape.h"

class EllipseShape : public PolygonShape
{
public:
	EllipseShape();

	void set_transform(sf::Vector2f position, sf::Vector2f size, sf::Angle rotation) override;
};
