#include <algorithm>
#include <ranges>

#include "selection_controller.h"

Selection* Selection::instance_ = nullptr;

Selection::Selection()
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

Selection::~Selection()
{
	for (auto transform_point : transform_points_)
		delete transform_point.second;
	transform_points_.clear();
}

void Selection::update_transform_data()
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
		selected_shapes_.size() == 1 ? selected_shapes_.front()->get_transform().rotation : sf::degrees(0.f) };
	selection_rect_.setPosition({ min_x, min_y });
	selection_rect_.setSize({ max_x - min_x, max_y - min_y });
}

void Selection::update_transform_points()
{
	float left = transform_data.position.x;
	float top = transform_data.position.y;
	float right = left + transform_data.size.x;
	float bottom = top + transform_data.size.y;

	float centerX = left + transform_data.size.x / 2.f;
	float centerY = top + transform_data.size.y / 2.f;

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
	}
}

Selection* Selection::get_instance() { return (instance_ == nullptr) ? instance_ = new Selection() : instance_; }

bool Selection::try_select_shape(BaseShape* shape, sf::Vector2f point, bool is_union)
{
	if (!shape->contains(point)) return false;
	try_add_figure_to_selection(shape, is_union);
	return true;
}

void Selection::try_add_figure_to_selection(BaseShape* shape, bool is_union)
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

void Selection::clear_selection()
{
	for (auto& shape : selected_shapes_)
		shape->set_outline(false);
	selected_shapes_.clear();
	is_selection_active_ = false;
}

void Selection::draw_selection(sf::RenderWindow& window)
{
	if (is_selection_active_) {
		window.draw(selection_rect_);
		for (auto transform_point : transform_points_)
			window.draw(*transform_point.second);
	}
}

void Selection::try_resize(sf::Vector2f mouse_position)
{
}

void Selection::resize_selected_shapes(sf::Vector2f mouse_position)
{
	sf::Vector2f delta = mouse_position - drag_start_;

	bool keep_aspect = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
	original_size()
	for (auto* shape : selected_shapes_)
	{
		TransformData t = shape->get_transform();

		t.size += delta;

		if (keep_aspect)
		{
			float ratio = original_size_.x / original_size_.y;
			t.size.y = t.size.x / ratio;
		}

		shape->set_transform(t);
	}

	update_transform_data();
	update_transform_points();
}

