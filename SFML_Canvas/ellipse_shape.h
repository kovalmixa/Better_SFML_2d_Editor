#pragma once
#include "base_shape.h"
#include "polygon_shape.h"

class EllipseShape : public PolygonShape
{
protected:
	void apply_transform() override;
public:
	EllipseShape();
};
