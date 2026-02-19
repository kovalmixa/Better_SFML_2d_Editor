#pragma once
#include <SFML/Graphics.hpp>

enum class ResizeMode {
    None,
    Angle,
    Center,
    TopLeft,
    Left,
    BottomLeft,
    TopRight,
	Right,
    BottomRight,
    Top,
    Bottom,
};

struct TransformData {
    sf::Vector2f position;
    sf::Vector2f size;
    sf::Angle rotation;
};

class BaseShape {
protected:
    sf::Shape* shape_ = nullptr;

	std::unordered_map<ResizeMode, sf::Shape*> transform_points_;
	const float TRANSFORM_POINTS_SIZE = 5.f;
    ResizeMode current_resize_ = ResizeMode::None;

    bool is_resizing_ = false;
	bool is_selected_ = false;

    virtual void update_transform_points();
    void set_pivot_center();
public:
    BaseShape();
    virtual ~BaseShape();

    virtual sf::Vector2f get_size();
    virtual sf::Vector2f get_position();
    virtual void set_transform(sf::Vector2f position, sf::Vector2f size, sf::Angle rotation) = 0;
    virtual sf::Color get_color();
    virtual void set_color(sf::Color color);

    virtual void draw(sf::RenderWindow& window);

    virtual bool contains(sf::Vector2f point) = 0;
    virtual bool try_select(sf::Vector2f point);
    void switch_selection();
    virtual void resize(sf::Vector2f delta) = 0;
};
