#pragma once
#include "base_shape.h"

class Polygon : public BaseShape
{
public:
	Polygon();

	void draw(sf::RenderWindow& window) override;
	virtual void set_transform(sf::Vector2f position, sf::Vector2f size, sf::Angle rotation) override;
	bool contains(sf::Vector2f point) override;
	void resize(sf::Vector2f delta) override;
};
