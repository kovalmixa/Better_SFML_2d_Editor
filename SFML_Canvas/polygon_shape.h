#pragma once
#include "base_shape.h"

class PolygonShape : public BaseShape
{
protected:
	int num_points_ = 3;

	void apply_transform() override;
	void setup_polygon_points();
public:
	PolygonShape(int points_quantity = 0);
	
	void set_points(const int points_quantity);
	bool contains(const sf::Vector2f point) override;
	BaseShape* clone() override;
};
