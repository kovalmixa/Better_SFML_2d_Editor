#pragma once
#include "base_shape.h"

class RectangleShape : public BaseShape
{
protected:
	void apply_transform() override;
public:
	RectangleShape();

	bool contains(sf::Vector2f point) override;
};