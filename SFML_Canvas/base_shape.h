#pragma once
#include <SFML/Graphics.hpp>
#include "transform_data.h"

class BaseShape {
protected:
    sf::Shape* shape_ = nullptr;
	TransformData transform_data_;

	virtual void apply_transform() = 0;
public:
    BaseShape();
    virtual ~BaseShape();

    virtual void set_transform(const sf::Vector2f position, const sf::Vector2f size, const sf::Angle rotation);
    virtual void set_transform(const TransformData transform_data);
    virtual TransformData get_transform();
    virtual sf::FloatRect get_global_bounds();

    virtual sf::Color get_color();
    virtual void set_color(const sf::Color color);
    void set_outline(const bool show);
    virtual void draw(sf::RenderWindow& window);

    virtual bool contains(const sf::Vector2f point) = 0;

    virtual BaseShape* clone() = 0;
};
