#pragma once
#include <list>
#include "base_shape.h"

enum class TransformMode {
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
    std::list<BaseShape*> selected_shapes_;
    sf::RectangleShape selection_rect_;
    TransformData transform_data;
	std::unordered_map<TransformMode, sf::Shape*> transform_points_;

	const float TRANSFORM_POINTS_SIZE = 5.f;
    bool is_resizing_ = false;
    bool is_selection_active_ = false;
    TransformMode active_transform_mode_ = TransformMode::None;

    Selection();
    ~Selection();

	void update_transform_data();
    void update_transform_points();
public:
    static Selection* get_instance();

	bool try_select_shape(BaseShape* shape, sf::Vector2f point, bool is_union);
    void try_add_figure_to_selection(BaseShape* shape, bool is_union);
	void clear_selection();

    void draw_selection(sf::RenderWindow& window);
    void try_resize(sf::Vector2f mouse_position);
	void resize_selected_shapes(sf::Vector2f mouse_position);
};