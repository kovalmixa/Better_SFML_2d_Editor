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

    virtual void set_transform(sf::Vector2f position, sf::Vector2f size, sf::Angle rotation);
    virtual void set_transform(TransformData transform_data);
    virtual TransformData get_transform();
    virtual sf::FloatRect get_global_bounds();

    virtual sf::Color get_color();
    virtual void set_color(sf::Color color);
    void set_outline(bool show);
    virtual void draw(sf::RenderWindow& window);

    virtual bool contains(sf::Vector2f point) = 0;
    virtual void resize(sf::Vector2f delta) = 0;

    virtual BaseShape* clone() = 0;
};
