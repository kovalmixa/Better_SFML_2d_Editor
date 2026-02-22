#pragma once
#include <list>
#include "base_shape.h"

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

class Selection {
private:
	static Selection* instance_;
    std::list<BaseShape*> selected_shapes;
	std::unordered_map<ResizeMode, sf::Shape*> transform_points_;
	const float TRANSFORM_POINTS_SIZE = 5.f;
	ResizeMode current_resize_ = ResizeMode::None;
	TransformData transform_data;
    bool is_resizing_ = false;
    bool is_selected_ = false;

    Selection();
    ~Selection();

    void update_transform_points();
public:
    Selection* get_instance();

	bool try_select_shape(BaseShape* shape, sf::Vector2f point);
    void try_add_figure_to_selection(BaseShape* shape);
	void clear_selection();

    void draw_selection(sf::RenderWindow& window);
    void try_resize(sf::Vector2f mouse_position);
	void resize_selected_shapes(sf::Vector2f mouse_position);
};