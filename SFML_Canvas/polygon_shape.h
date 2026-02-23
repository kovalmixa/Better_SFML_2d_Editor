#pragma once
#include "base_shape.h"

class PolygonShape : public BaseShape
{
protected:
	int num_points_ = 3;

	virtual void apply_transform() override;
	void setup_polygon_points();
public:
	PolygonShape();
	
	void set_points(const std::vector<sf::Vector2f>& points);
	bool contains(sf::Vector2f point) override;
	void resize(sf::Vector2f delta) override;
};
