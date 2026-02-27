#pragma once
#include "base_shape.h"

class PolygonShape : public BaseShape
{
protected:
	int num_points_ = 3;

	virtual void apply_transform() override;
	void set_size();
	void setup_polygon_points(int points_quantity);
public:
	PolygonShape(int points_quantity = 0);
	
	void set_points(const std::vector<sf::Vector2f>& points);
	bool contains(sf::Vector2f point) override;
	BaseShape* clone() override;
};
