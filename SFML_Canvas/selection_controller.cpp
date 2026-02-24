#include <algorithm>
#include <ranges>

#include "basic_functions.h"
#include "selection_controller.h"

SelectionController* SelectionController::instance_ = nullptr;

SelectionController::SelectionController()
{
	selection_rect_ = sf::RectangleShape();
	selection_rect_.setFillColor(sf::Color::Transparent);
	selection_rect_.setOutlineColor(sf::Color::Blue);
	selection_rect_.setOutlineThickness(1.f);

	transform_points_[TransformMode::Angle] = new sf::ConvexShape(3);
	auto triangle = dynamic_cast<sf::ConvexShape*>(transform_points_[TransformMode::Angle]);
	triangle->setPoint(0, { TRANSFORM_POINTS_SIZE, TRANSFORM_POINTS_SIZE });
	triangle->setPoint(1, { TRANSFORM_POINTS_SIZE / 2 , 0 });
	triangle->setPoint(2, { 0, TRANSFORM_POINTS_SIZE });
	triangle->scale({ 2, 2 });
	//triangle->setRotation(sf::degrees(180.f));

	transform_points_[TransformMode::Center] = new sf::RectangleShape({ TRANSFORM_POINTS_SIZE, TRANSFORM_POINTS_SIZE });
	transform_points_[TransformMode::Center]->setRotation(sf::degrees(45.f));
	transform_points_[TransformMode::Center]->scale({ 2, 2 });

	transform_points_[TransformMode::TopLeft] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[TransformMode::Left] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[TransformMode::BottomLeft] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[TransformMode::TopRight] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[TransformMode::Right] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[TransformMode::BottomRight] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[TransformMode::Top] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[TransformMode::Bottom] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);

	for (auto point : transform_points_) {
		point.second->setFillColor(sf::Color::White);
		point.second->setOutlineColor(sf::Color::Blue);
		point.second->setOutlineThickness(1.f);
	}
}

SelectionController::~SelectionController()
{
	for (auto transform_point : transform_points_)
		delete transform_point.second;
	transform_points_.clear();
}

void SelectionController::update_transform_data()
{
	if (selected_shapes_.empty()) return;
	float min_x = std::numeric_limits<float>::max();
	float min_y = std::numeric_limits<float>::max();
	float max_x = std::numeric_limits<float>::lowest();
	float max_y = std::numeric_limits<float>::lowest();

	for (auto* shape : selected_shapes_)
	{
		auto bounds = shape->get_global_bounds();
		min_x = std::min(min_x, bounds.position.x);
		min_y = std::min(min_y, bounds.position.y);
		max_x = std::max(max_x, bounds.position.x + bounds.size.x);
		max_y = std::max(max_y, bounds.position.y + bounds.size.y);
	}
	transform_data = { { min_x, min_y }, { max_x - min_x, max_y - min_y }, 
		selected_shapes_.size() == 1 ? selected_shapes_.front()->get_transform().rotation : transform_data.rotation };
	selection_rect_.setPosition({ min_x, min_y });
	selection_rect_.setSize({ max_x - min_x, max_y - min_y });
}

void SelectionController::update_transform_points()
{
	float left = transform_data.position.x;
	float top = transform_data.position.y;
	float right = left + transform_data.size.x;
	float bottom = top + transform_data.size.y;

	float centerX = left + transform_data.size.x / 2.f;
	float centerY = top + transform_data.size.y / 2.f;

	selection_rect_.setPosition({ left, top });
	selection_rect_.setSize(transform_data.size);
	selection_rect_.setRotation(transform_data.rotation);


	for (auto transform_point : transform_points_) {
		auto shape = transform_point.second;
		switch (transform_point.first)
		{
		case TransformMode::Angle: shape->setPosition({ centerX, top - 42 }); break;
		case TransformMode::Center: shape->setPosition({ centerX, centerY }); break;
		case TransformMode::TopLeft: shape->setPosition({ left, top }); break;
		case TransformMode::Left: shape->setPosition({ left, centerY }); break;
		case TransformMode::BottomLeft: shape->setPosition({ left, bottom }); break;
		case TransformMode::TopRight: shape->setPosition({ right, top }); break;
		case TransformMode::Right: shape->setPosition({ right, centerY }); break;
		case TransformMode::BottomRight: shape->setPosition({ right, bottom }); break;
		case TransformMode::Top: shape->setPosition({ centerX, top }); break;
		case TransformMode::Bottom: shape->setPosition({ centerX, bottom }); break;
		default: break;
		}
		set_pivot_center_to_shape(*shape);
	}
}

bool SelectionController::is_point_near_transform_point(sf::Vector2f point, sf::Shape* transform_point)
	{ return euclidean_distance(point, transform_point->getPosition()) <= TRANSFORM_POINTS_DRAG_RADIUS_OFFSET; } 

void SelectionController::drag_rotation(sf::Vector2f mouse_position)
{
	auto center = transform_data.position + transform_data.size / 2.f;
	auto vector_to_mouse = get_vector_from_points(center, mouse_position);
	auto vector_to_rotation_point = get_vector_from_points(center, transform_points_[TransformMode::Angle]->getPosition());
	float angle = angle_between_vectors_2d(vector_to_rotation_point, vector_to_mouse);
	transform_data.rotation += sf::degrees(angle);
	update_transform_to_selected_shapes();
}

void SelectionController::drag_movement(sf::Vector2f mouse_position)
{
	auto delta = get_vector_from_points(transform_data.position + transform_data.size / 2.f, mouse_position);
	transform_data.position += delta;
	update_transform_to_selected_shapes();
}

void SelectionController::drag_resize(sf::Vector2f mouse_position, TransformMode trans_mode)
{
	auto transform_point = transform_points_[trans_mode];
	auto center_to_point = get_vector_from_points(transform_data.position + transform_data.size / 2.f, transform_point->getPosition());
	auto point_to_mouse = get_vector_from_points(transform_point->getPosition(), mouse_position);
	auto delta = point_to_mouse - center_to_point;

	if (trans_mode == TransformMode::Left || trans_mode == TransformMode::Right) transform_data.size.x += delta.x;
	else if (trans_mode == TransformMode::Top || trans_mode == TransformMode::Bottom) transform_data.size.y += delta.y;
	else transform_data.size += delta;
	update_transform_to_selected_shapes();
}

void SelectionController::drag_selection(sf::Vector2f mouse_position)
{
}

void SelectionController::update_transform_to_selected_shapes()
{
	auto delta_position = last_transform_data.position - transform_data.position;
	auto delta_size = last_transform_data.size - transform_data.size;
	auto delta_rotation = last_transform_data.rotation - transform_data.rotation;

	for (auto* shape : selected_shapes_)
	{
		auto shape_transform = shape->get_transform();
		shape_transform.position -= delta_position;
		shape_transform.size -= delta_size;
		shape_transform.rotation -= delta_rotation;
		shape->set_transform(shape_transform);
	}
	update_transform_points();
}

SelectionController* SelectionController::get_instance() { return (instance_ == nullptr) ? instance_ = new SelectionController() : instance_; }

bool SelectionController::try_select_shape(BaseShape* shape, sf::Vector2f point, bool is_union)
{
	if (!shape->contains(point)) return false;
	try_add_figure_to_selection(shape, is_union);
	return true;
}

void SelectionController::try_add_figure_to_selection(BaseShape* shape, bool is_union)
{
	if (!is_union) clear_selection();
	bool is_selected = std::find(selected_shapes_.begin(), selected_shapes_.end(), shape) != selected_shapes_.end();
	if (!is_selected) selected_shapes_.push_back(shape);
	else {
		selected_shapes_.remove(shape);
		shape->set_outline(false);
	}
	is_selection_active_ = !(selected_shapes_.size() == 0);
	if (is_selection_active_) {
		update_transform_data();
		update_transform_points();
	}
	for (auto& shape : selected_shapes_)
		shape->set_outline(is_selection_active_);
}

void SelectionController::clear_selection()
{
	for (auto& shape : selected_shapes_)
		shape->set_outline(false);
	selected_shapes_.clear();
	is_selection_active_ = false;
}

void SelectionController::begin_drag_transform_mode(sf::Vector2f mouse_position)
{
	if (!is_selection_active_) return;
	for (auto trans_point : transform_points_)
	{
		if (is_point_near_transform_point(mouse_position, trans_point.second))
		{
			active_transform_mode_ = trans_point.first;
			is_transforming_ = true;
			return;
		}
	}
	if (selection_rect_.getGlobalBounds().contains(mouse_position))
		active_transform_mode_ = TransformMode::Center;
	else active_transform_mode_ = TransformMode::None;
}

void SelectionController::end_drag_transform_mode()
{
	is_transforming_ = false;
	active_transform_mode_ = TransformMode::None;
}

void SelectionController::update_transform(sf::Vector2f mouse_position)
{
	if (!is_selection_active_ || !is_transforming_) return;
	last_transform_data = transform_data;
	switch (active_transform_mode_)
	{
	case TransformMode::Angle: drag_rotation(mouse_position); break;
	case TransformMode::Center: drag_movement(mouse_position); break;
	default: drag_resize(mouse_position, active_transform_mode_); break;
	}
}

void SelectionController::draw_selection(sf::RenderWindow& window)
{
	if (is_selection_active_) {
		window.draw(selection_rect_);
		for (auto transform_point : transform_points_)
			window.draw(*transform_point.second);
	}
}
