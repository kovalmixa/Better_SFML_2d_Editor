#pragma once
#include <SFML/Graphics.hpp>
#include "shape_transform.h"

enum class ResizeMode {
    None,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
};

class BaseShape {
protected:
    sf::Shape* shape_ = nullptr;
    sf::Shape* hovered_shape_ = nullptr;
	ShapeTransform transform_;

    ResizeMode current_resize_ = ResizeMode::None;
    bool is_resizing_ = false;
	bool is_selected_ = false;

public:
    BaseShape();
    virtual ~BaseShape();

    virtual void set_transform(sf::Vector2f position, sf::Vector2f size, float rotation);
    virtual void set_color(sf::Color color);

    virtual void draw(sf::RenderWindow& window) = 0;

    virtual bool contains(sf::Vector2f point) = 0;
    virtual void try_select(sf::Vector2f point);
    virtual void resize(sf::Vector2f delta) = 0;
    virtual sf::FloatRect getBounds() = 0;
};
