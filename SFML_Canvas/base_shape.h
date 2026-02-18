#pragma once
#include <SFML/Graphics.hpp>

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

    ResizeMode current_resize_ = ResizeMode::None;
    bool is_resizing_ = false;
	bool is_selected_ = false;

	virtual void setup_transform_points();
public:
    BaseShape();
    virtual ~BaseShape();

    virtual void set_transform(sf::Vector2f position, sf::Vector2f size, sf::Angle rotation);
    virtual void update_transform_points();
    virtual void set_color(sf::Color color);

    virtual void draw(sf::RenderWindow& window) = 0;

    virtual bool contains(sf::Vector2f point) = 0;
    virtual void try_select(sf::Vector2f point);
    virtual void resize(sf::Vector2f delta) = 0;

    void set_pivot_center();
};
