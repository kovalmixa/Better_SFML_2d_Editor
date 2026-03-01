#pragma once
#include "base_shape.h"

class EllipseShape : public BaseShape
{
protected:
	const int POINTS_NUM = 30;
	void apply_transform() override;
public:
	EllipseShape();

	bool contains(const sf::Vector2f point) override;
	BaseShape* clone() override;
};