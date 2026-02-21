#pragma once
#include <list>
#include "base_shape.h"

class selection {
private:
	std::unordered_map<ResizeMode, sf::Shape*> transform_points_;
	const float TRANSFORM_POINTS_SIZE = 5.f;
	ResizeMode current_resize_ = ResizeMode::None;

	std::list<BaseShape*> selected_shapes;
public:
};