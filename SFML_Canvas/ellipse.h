#pragma once
#include "base_shape.h"
#include "polygon.h"

class Ellipse : public Polygon
{
public:
	Ellipse();

	void set_transform(sf::Vector2f position, sf::Vector2f size, sf::Angle rotation) override;
};
