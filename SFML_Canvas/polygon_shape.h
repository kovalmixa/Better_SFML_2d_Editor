#pragma once
#include "base_shape.h"

class PolygonShape : public BaseShape
{
protected:
	float width_ = 1;
	float height_ = 1;
	int num_points_ = 3;

	void setup_polygon_points();
public:
	PolygonShape();
	
	void set_points(const std::vector<sf::Vector2f>& points);
	virtual void set_transform(sf::Vector2f position, sf::Vector2f size, sf::Angle rotation) override;
	bool contains(sf::Vector2f point) override;
	void resize(sf::Vector2f delta) override;
};
