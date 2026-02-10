#pragma once
#include "base_shape.h"

class Rectangle : public BaseShape
{
public:
	Rectangle();

	void draw(sf::RenderWindow& window) override;
	bool contains(sf::Vector2f point) override;
	void resize(sf::Vector2f delta) override;
};