#pragma once
#include "base_shape.h"

class RectangleShape : public BaseShape
{
public:
	RectangleShape();

	void set_transform(sf::Vector2f position, sf::Vector2f size, sf::Angle rotation) override;
	bool contains(sf::Vector2f point) override;
	void resize(sf::Vector2f delta) override;
};